
#include "reg_rcc.h"
#include "reg_usart.h"
#include "reg_gpio.h"

#include "syscalls.h"
#include "ProjectVersion.h"

#include "menu.h"

#include <rtc.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>

/* Standard includes. */
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define COMMAND_BUFFER_SIZE 30
#define COMMAND_TASK_STACK_SIZE 512U


typedef enum {                 //
    GPIO_DIRECTION_INPUT = 0,  //
    GPIO_DIRECTION_OUTPUT = 1  //
} GpioDirection;

static void gpioInit(uint32_t pin, GpioDirection direction);
static void uart2Init(void);

static void uart2Put(uint8_t data);
static uint8_t uart2Get(void);

static void writePin(uint32_t pin, bool high);

static void runPinSequence(void);
static void myDelay(int tck);
static void example0Task(void* parameters);
static void example1Task(void* parameters);

static void commandLineTask(void* parameters);

void uart_putc(uint8_t ch);
uint8_t uart_getc(void);


void uart_putc(uint8_t ch) {
    uart2Put(ch);
}

uint8_t uart_getc(void) {
    return uart2Get();
}

static void gpioInit(const uint32_t pin, GpioDirection direction) {
    if (pin > 15U) {
        return;
    }

    /* Enable the GPIOA peripheral clock. */
    RCC->ahb1enr |= (uint32_t)(1UL << 0U);
    (void)RCC->ahb1enr;

    uint32_t modeShift = pin * 2U;
    uint32_t modeMask = (uint32_t)(0x3UL << modeShift);

    /* Configure the selected GPIOA pin as an input (00) or output (01). */
    GPIOA->moder = (GPIOA->moder & ~modeMask) | ((uint32_t)direction & 0x1UL) << modeShift;
}

static void uart2Init(void) {
    const uint32_t pins = (1UL << 2U) | (1UL << 3U);

    /*
     * PA2 = USART2_TX and
     * PA3 = USART2_RX (alternate function 7).
     */
    RCC->ahb1enr |= (uint32_t)(1UL << 0U);
    (void)RCC->ahb1enr;

    GPIOA->moder = (GPIOA->moder & ~((3UL << 4U) | (3UL << 6U))) | (2UL << 4U) | (2UL << 6U);

    GPIOA->afrl = (GPIOA->afrl & ~((15UL << 8U) | (15UL << 12U))) | (7UL << 8U) | (7UL << 12U);

    GPIOA->otyper = (GPIOA->otyper & ~pins);

    GPIOA->ospeedr = (GPIOA->ospeedr & ~((3UL << 4U) | (3UL << 6U))) | (2UL << 4U) | (2UL << 6U);

    GPIOA->pupdr = (GPIOA->pupdr & ~((3UL << 4U) | (3UL << 6U))) | (1UL << 4U) | (1UL << 6U);

    /* Configure USART2 for 115200 baud, 8 data bits, no parity and one stop bit.
     * APB1 runs at 50 MHz, so BRR = round(50000000 / 115200) = 434. */
    RCC->apb1enr |= (uint32_t)(1UL << 17U);
    (void)RCC->apb1enr;

    USART2->cr1 = 0UL;
    USART2->cr2 = 0UL;
    USART2->cr3 = 0UL;

    USART2->brr = 434UL;
    USART2->cr1 = (1UL << 13U) | (1UL << 3U) | (1UL << 2U);
}

static void uart2Put(uint8_t data) {
    /* Wait until the transmit data register is empty. */
    while ((USART2->sr & (1UL << 7U)) == 0UL) {
    }

    USART2->dr = (uint32_t)data;
}

static uint8_t uart2Get(void) {
    /*
     * Wait until a byte has been received.
     * Reading DR clears RXNE.
     */
    while ((USART2->sr & (1UL << 5U)) == 0UL) {
    }

    return (uint8_t)(USART2->dr & 0xFFUL);
}

static void writePin(const uint32_t pin, const bool high) {
    if (pin > 15U) {
        return;
    }

    if (high) {
        GPIOA->bsrr = (uint32_t)(1UL << pin);
    } else {
        GPIOA->bsrr = (uint32_t)(1UL << (pin + 16U));
    }
}

static void runPinSequence(void) {
    writePin(5U, true);
    writePin(6U, true);
    writePin(7U, true);

    vTaskDelay(pdMS_TO_TICKS(1500U));

    writePin(6U, false);
    writePin(7U, false);
}

static void example0Task(void* parameters) {
    (void)parameters;

    bool toggle = false;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1300));  // 1.3 sec
        writePin(6U, toggle);

        toggle = (toggle) ? false : true;
    }
}

static void example1Task(void* parameters) {
    (void)parameters;

    bool toggle = false;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(800));  // 0.8 sec

        writePin(7U, toggle);

        toggle = (toggle) ? false : true;
    }
}

static void myDelay(int tck) {
    vTaskDelay(pdMS_TO_TICKS(tck));
}

static void commandLineTask(void* parameters) {
    (void)parameters;

    static char buffer[COMMAND_BUFFER_SIZE + 1];

    ME_initCommandBuffer(buffer, COMMAND_BUFFER_SIZE);

    for (;;) {
        int cmd_index = ME_command(buffer, COMMAND_BUFFER_SIZE, myDelay, 10);

        int rc = ME_commandProcess(buffer, cmd_index);
        if (rc) {
            ME_commandError(ERR_BADCMD, NULL);
        }

        if (rc == 0) {
            printf("\n");
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

#if 0

void vProducerTask(void* pvParameters);
void vConsumerTask(void* pvParameters);

typedef struct {
    int id;
    int count;
} SensorData;

QueueHandle_t xDataQueue = NULL;

void vProducerTask(void* pvParameters) {
    SensorData sensorData;
    sensorData.count = 0;

    BaseType_t xStatus;

    bool toggle = true;
    while (1) {
        sensorData.count++;
        printf("Producer: Sending value %d to queue...\n", sensorData.count);

        // Send value to the back of the queue.
        // Wait up to 10 ticks if the queue happens to be full.
        xStatus = xQueueSendToBack(xDataQueue, (void*)&sensorData, pdMS_TO_TICKS(100));

        if (xStatus != pdPASS) {
            printf("Producer: Could not send data to queue (Queue Full).\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));

        writePin(6U, toggle);
        toggle = (toggle) ? false : true;
    }
}

void vConsumerTask(void* pvParameters) {
    SensorData sensorData;
    BaseType_t xStatus;

    bool toggle = true;
    writePin(5U, toggle);

    while (1) {
        // Block indefinitely (portMAX_DELAY) until an item is available
        xStatus = xQueueReceive(xDataQueue, (void *)&sensorData, portMAX_DELAY);

        if (xStatus == pdPASS) {
            printf("Consumer: Successfully received value %d\n", sensorData.count);
        }

        writePin(5U, toggle);
        toggle = (toggle) ? false : true;
    }
}
#endif

int main(void) {
    const char* p = projectVersion;

    /* Task Control Block TCB */
    static StaticTask_t exampleTask0TCB;
    static StaticTask_t exampleTask1TCB;
    static StaticTask_t exampleTask2TCB;

    static StackType_t exampleTask0Stack[configMINIMAL_STACK_SIZE];
    static StackType_t exampleTask1Stack[configMINIMAL_STACK_SIZE];
    static StackType_t exampleTask2Stack[COMMAND_TASK_STACK_SIZE];

    uart2Init();

    gpioInit(5U, GPIO_DIRECTION_OUTPUT);
    gpioInit(6U, GPIO_DIRECTION_OUTPUT);
    gpioInit(7U, GPIO_DIRECTION_OUTPUT);

    writePin(5U, false);
    writePin(6U, false);
    writePin(7U, false);

    (void)printf("\n\nSTM32F411 SW ver=%s\n", p);

    rtc_init();

    rtc_time_t t = rtc_get_time();

    (void)printf("%02d:%02d:%02d\n", t.hour, t.minute, t.second);

#if 0
    { /* Test floating point */
        double a = 3.141592;
        (void)printf("Pi is: %10.8lf\n", a);

        for (int i = 0; i < 20; i++) {
            a = a + 0.13;

            (void)printf("%d delta added to pi: %10.8lf\n", i, a);
        }
    }
#endif

#if 0
    xDataQueue = xQueueCreate(5, sizeof(SensorData));

    if (xDataQueue == NULL) {
        printf("Error: Failed to create the queue due to insufficient heap.\n");
        for (;;) {
            ;
        }
    }

    xTaskCreate(vProducerTask, "Producer", 256, NULL, 1, NULL);

    xTaskCreate(vConsumerTask, "Consumer", 256, NULL, 2, NULL);
#endif

    (void)xTaskCreateStatic(example0Task,
                            "example0",
                            configMINIMAL_STACK_SIZE,
                            NULL,
                            configMAX_PRIORITIES - 1U,
                            &(exampleTask0Stack[0]),
                            &(exampleTask0TCB));

    (void)xTaskCreateStatic(example1Task,
                            "example1",
                            configMINIMAL_STACK_SIZE,
                            NULL,
                            configMAX_PRIORITIES - 1U,
                            &(exampleTask1Stack[0]),
                            &(exampleTask1TCB));

    (void)xTaskCreateStatic(commandLineTask,
                            "commandline",
                            COMMAND_TASK_STACK_SIZE,
                            NULL,
                            configMAX_PRIORITIES - 1U,
                            &(exampleTask2Stack[0]),
                            &(exampleTask2TCB));

    /* Start the scheduler. */
    vTaskStartScheduler();

    for (;;) {
        /* Should not reach here. */
    }

    return 0;
}


#if (configCHECK_FOR_STACK_OVERFLOW > 0)
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {
    /* Check pcTaskName for the name of the offending task,
     * or pxCurrentTCB if pcTaskName has itself been corrupted. */
    (void)xTask;
    (void)pcTaskName;
}
#endif

#if (configUSE_MALLOC_FAILED_HOOK == 1)
void vApplicationMallocFailedHook(void) {
    __asm volatile("cpsid i" ::: "memory");
    for (;;) {
    }
}
#endif

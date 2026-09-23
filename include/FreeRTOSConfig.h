#pragma once

#include <stdint.h>

/*
 * STM32F411CEU6 (Black Pill)
 * Cortex-M4F
 *
 * Adjust configCPU_CLOCK_HZ to match your actual HCLK.
 *
 * Examples:
 *   16 MHz HSI:
 *       16000000UL
 *
 *   100 MHz system clock:
 *       100000000UL
 */

/*-----------------------------------------------------------
 * Scheduler
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION 1
#define configUSE_TIME_SLICING 1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

#define configCPU_CLOCK_HZ 100000000UL
#define configTICK_RATE_HZ 1000U

#define configMAX_PRIORITIES 8

#define configMINIMAL_STACK_SIZE 128U
#define configMAX_TASK_NAME_LEN 16

#define configUSE_16_BIT_TICKS 0

#define configIDLE_SHOULD_YIELD 1

/*-----------------------------------------------------------
 * Memory
 *----------------------------------------------------------*/

#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION 1
#define configKERNEL_PROVIDED_STATIC_MEMORY 1

/*
 * Used by heap_1.c, heap_2.c, heap_4.c, etc.
 *
 * The STM32F411CEU6 has 128 KiB of SRAM.
 */
#define configTOTAL_HEAP_SIZE (32U * 1024U)

/*-----------------------------------------------------------
 * Hooks
 *----------------------------------------------------------*/

#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0

#define configUSE_MALLOC_FAILED_HOOK 1
#define configCHECK_FOR_STACK_OVERFLOW 2

/*-----------------------------------------------------------
 * Synchronization
 *----------------------------------------------------------*/

#define configUSE_MUTEXES 1
#define configUSE_RECURSIVE_MUTEXES 1
#define configUSE_COUNTING_SEMAPHORES 1

#define configQUEUE_REGISTRY_SIZE 8

/*-----------------------------------------------------------
 * Software timers
 *----------------------------------------------------------*/

#define configUSE_TIMERS 1

#define configTIMER_TASK_PRIORITY 2
#define configTIMER_QUEUE_LENGTH 10
#define configTIMER_TASK_STACK_DEPTH 256

/*-----------------------------------------------------------
 * Cortex-M interrupt priorities
 *----------------------------------------------------------*/

/*
 * STM32F411 implements 4 NVIC priority bits.
 *
 * Valid CMSIS interrupt priority numbers:
 *
 *     0  = highest priority
 *     15 = lowest priority
 */
#define configPRIO_BITS 4

/*
 * FreeRTOS kernel interrupts use the lowest priority.
 */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15

/*
 * Interrupts with numerical priorities 5..15 may call
 * FreeRTOS xxxFromISR() functions.
 *
 * Interrupts 0..4 must NOT call FreeRTOS API functions.
 */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/*
 * Convert CMSIS-style priority values into the hardware
 * priority register representation.
 */
#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

/*-----------------------------------------------------------
 * API functions
 *----------------------------------------------------------*/

#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_uxTaskPriorityGet 1

#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskSuspend 1

#define INCLUDE_vTaskDelay 1
#define INCLUDE_vTaskDelayUntil 1

#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_xTaskGetCurrentTaskHandle 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1

/*-----------------------------------------------------------
 * Optional diagnostics
 *----------------------------------------------------------*/

#define configUSE_TRACE_FACILITY 1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1

/*-----------------------------------------------------------
 * Cortex-M exception handlers
 *----------------------------------------------------------*/

/*
 * Let the FreeRTOS Cortex-M4F port provide these handlers.
 *
 * Your startup vector table must use:
 *
 *     SVC_Handler
 *     PendSV_Handler
 *     SysTick_Handler
 */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/* Enable both static and dynamic stack allocation for tasks */
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION  1

/*-----------------------------------------------------------
 * Assertions
 *----------------------------------------------------------*/

/*
 * For development/debugging.
 */
#define configASSERT(x)      \
    do {                     \
        if ((x) == 0) {      \
            __asm volatile("cpsid i" ::: "memory"); \
            for (;;) {       \
            }                \
        }                    \
    } while (0)

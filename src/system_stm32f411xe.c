#include <stdint.h>

#include "reg_rcc.h"
#include "reg_rtc.h"
#include "reg_flash.h"
#include "reg_pwr.h"


#define SCB_CPACR (*(volatile uint32_t*)(0xE000ED88UL))
#define SCB_VTOR  (*(volatile uint32_t*)(0xE000ED08UL))

extern const uint32_t g_pfnVectors[];

void SystemInit(void);

/* Configure the STM32F411 for 100 MHz from the 16 MHz internal oscillator. */
void SystemInit(void) {
    /* Enable CP10 and CP11 so the Cortex-M4 floating-point unit is usable. */
    SCB_CPACR |= (0xFUL << 20U);

    /* Enable HSI and wait until it is stable. */
    RCC->cr |= 1UL;
    while ((RCC->cr & (1UL << 1U)) == 0UL) {
    }

    /* Reset the clock tree before changing the PLL. */
    RCC->cfgr = 0UL;
    RCC->cr &= ~((1UL << 16U) | (1UL << 24U));

    while ((RCC->cr & (1UL << 25U)) != 0UL) {
    }

    /* Voltage scale 1 is required to run the STM32F411 at 100 MHz. */
    RCC->apb1enr |= (1UL << 28U);
    PWR->cr |= (1UL << 14U);

    /* HSI / 16 * 400 / 4 = 100 MHz. PLLQ=8 produces a 50 MHz domain clock. */
    RCC->pllcfgr = 16UL | (400UL << 6U) | (1UL << 16U) | (8UL << 24U);

    /* Three wait states, instruction cache, data cache and prefetch. */
    FLASH->acr = 3UL | (1UL << 8U) | (1UL << 9U) | (1UL << 10U);

    RCC->cr |= (1UL << 24U);
    while ((RCC->cr & (1UL << 25U)) == 0UL) {
    }

    /* AHB=100 MHz, APB1=50 MHz, APB2=100 MHz, system clock=PLL. */
    RCC->cfgr = (4UL << 10U) | 2UL;
    while ((RCC->cfgr & (3UL << 2U)) != (2UL << 2U)) {
    }

    SCB_VTOR = (uint32_t)g_pfnVectors;
}

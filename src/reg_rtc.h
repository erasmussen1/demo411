#pragma once

#include "reg_map.h"

#include <stdint.h>

typedef struct {
    volatile uint32_t tr;       // Time register
    volatile uint32_t dr;       // Date register
    volatile uint32_t cr;       // Control register
    volatile uint32_t isr;      // Initialization/status register
    volatile uint32_t prer;     // Pre-scaler register
    volatile uint32_t wutr;     // Wakeup timer register
    volatile uint32_t calibr;
    volatile uint32_t alrmar;
    volatile uint32_t alrmbr;
    volatile uint32_t wpr;      // Write protection register
    volatile uint32_t ssr;
    volatile uint32_t shiftr;
    volatile uint32_t tstr;
    volatile uint32_t tsdr;
    volatile uint32_t tsssr;
    volatile uint32_t calr;
    volatile uint32_t tafcr;
    volatile uint32_t alrmassr;
    volatile uint32_t alrmbssr;
    uint32_t RESERVED7;
    volatile uint32_t bkp0r;
    volatile uint32_t bkp1r;
    /* ... backup registers ... */
} RTC_TypeDef;

#define RTC         ((RTC_TypeDef *) RTC_BASE)



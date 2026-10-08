
#pragma once

#include "reg_map.h"

#include <stdint.h>


typedef struct {                //  Offset	// Description
    volatile uint32_t moder;    // 0x00
    volatile uint32_t otyper;   // 0x04
    volatile uint32_t ospeedr;  // 0x08
    volatile uint32_t pupdr;    // 0x0C
    volatile uint32_t idr;      // 0x10
    volatile uint32_t odr;      // 0x14
    volatile uint32_t bsrr;     // 0x18
    volatile uint32_t lckr;     // 0x1C
    volatile uint32_t afrl;     // 0x20
    volatile uint32_t afrh;     // 0x24
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef*)GPIOC_BASE)
#define GPIOD ((GPIO_TypeDef*)GPIOD_BASE)
#define GPIOE ((GPIO_TypeDef*)GPIOE_BASE)
#define GPIOH ((GPIO_TypeDef*)GPIOH_BASE)

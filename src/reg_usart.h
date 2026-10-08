#pragma once

#include "reg_map.h"

#include <stdint.h>

typedef struct {
    volatile uint32_t sr;    // 0x00
    volatile uint32_t dr;    // 0x04
    volatile uint32_t brr;   // 0x08
    volatile uint32_t cr1;   // 0x0C
    volatile uint32_t cr2;   // 0x10
    volatile uint32_t cr3;   // 0x14
    volatile uint32_t gtpr;  // 0x18
} USART_TypeDef;

#define USART1 ((USART_TypeDef*)USART1_BASE)
#define USART2 ((USART_TypeDef*)USART2_BASE)

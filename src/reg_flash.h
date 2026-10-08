#pragma once

#include "reg_map.h"

#include <stdint.h>

typedef struct {
    volatile uint32_t acr;
} FLASH_TypeDef;

#define FLASH ((FLASH_TypeDef*)FLASH_BASE)

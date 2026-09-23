#pragma once

#include "reg_map.h"

#include <stdint.h>


typedef struct {	    			//  Offset	// Description
	volatile uint32_t cr;	    	//	0x00
	volatile uint32_t csr;	    	//	0x04
} PWR_TypeDef;

#define PWR         ((PWR_TypeDef *) PWR_BASE)


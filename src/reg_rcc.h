#pragma once

#include "reg_map.h"

#include <stdint.h>


typedef struct {                 //  Offset	// Description
    volatile uint32_t cr;        //	0x00	// clock control register
    volatile uint32_t pllcfgr;   //	0x04	// pll configuration register
    volatile uint32_t cfgr;      //	0x08	// clock configuration register
    volatile uint32_t cir;       //	0x0c	// clock interrupt register
    volatile uint32_t ahb1rstr;  //	0x10	// ahb1 peripheral reset register
    volatile uint32_t ahb2rstr;  //	0x14	// ahb2 peripheral reset register
    volatile uint32_t ahb3rstr;  //	0x18	// ahb3 peripheral reset register
    volatile uint32_t rev0;      //	0x1c	// reversed0
    volatile uint32_t apb1rstr;  //	0x20	// apb1 peripheral reset register
    volatile uint32_t apb2rstr;  //	0x24	// apb2 peripheral reset register
    volatile uint32_t rev1[2];   //	0x28/ 0x2c	//	reversed1 for the two locations
    volatile uint32_t ahb1enr;   //	0x30	// ahb1 peripheral clock enable register
    volatile uint32_t ahb2enr;   //	0x34	// ahb2 peripheral clock enable register
    volatile uint32_t ahb3enr;   //	0x38	// ahb3 peripheral clock enable register
    volatile uint32_t rev2;      //	0x3c	// reversed2
    volatile uint32_t apb1enr;   //	0x40	// apb1 peripheral clock enable register
    volatile uint32_t apb2enr;   //	0x44	// apb2 peripheral clock enable register
    volatile uint32_t rev3[2];   //	0x48/ 0x4c	//	reversed3 for the two locations
    volatile uint32_t
        ahb1lpenr;  //	0x50	// ahb1 peripheral clock enable in low power mode register
    volatile uint32_t
        ahb2lpenr;  //  0x54	// ahb2 peripheral clock enable in low power mode register
    volatile uint32_t
        ahb3lpenr;           //  0x58	// ahb3 peripheral clock enable in low power mode register
    volatile uint32_t rev4;  //  0x5c	// reversed4
    volatile uint32_t
        apb1lpenr;  //  0x60	// apb1 peripheral clock enable in low power mode register
    volatile uint32_t
        apb12penr;              //  0x64	// apb2 peripheral clock enable in low power mode register
    volatile uint32_t rev5[2];  //	0x68/ 0x6c	//	reversed5 for the two locations
    volatile uint32_t bdcr;     //	0x70	// backup domain control register
    volatile uint32_t csr;      //	0x74	// clock control & status register
    volatile uint32_t rev6[2];  //	0x78/ 0x7c	//	reversed6 for the two locations
    volatile uint32_t sscgr;    //	0x80	// spread spectrum clock generation register
    volatile uint32_t plli2scfgr;  //	0x84	// plli2scfgr configuration register
} RCC_TypeDef;

#define RCC ((RCC_TypeDef*)RCC_BASE)

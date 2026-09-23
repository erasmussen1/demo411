#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


uint8_t bcd_to_bin(uint8_t bcd);

uint8_t bin_to_bcd(uint8_t value);

uint32_t ymdw2dr(uint8_t year,    //
                 uint8_t month,   //
                 uint8_t day,     //
                 uint8_t weekday  //
);

int dr2ymdw(const uint32_t dr,  //
            uint8_t* year,      //
            uint8_t* month,     //
            uint8_t* day,       //
            uint8_t* weekday    //
);

uint8_t rtc_weekday(uint16_t year, uint8_t month, uint8_t day);

int trim4timeDate(char* buf, int* len);

#ifdef __cplusplus
}
#endif

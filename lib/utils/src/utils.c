
#include "utils.h"

#include <stdint.h>

uint8_t bcd_to_bin(uint8_t bcd) {
    return (uint8_t)(((bcd >> 4) * 10) + (bcd & 0x0F));
}

uint8_t bin_to_bcd(uint8_t value) {
    return (uint8_t)(((value / 10) << 4) | (value % 10));
}

uint32_t ymdw2dr(uint8_t year,    //
                 uint8_t month,   //
                 uint8_t day,     //
                 uint8_t weekday  //
) {
    /* clang-format off */
    return ((uint32_t)(year / 10)  << 20) |
           ((uint32_t)(year % 10)  << 16) |
           ((uint32_t)weekday      << 13) |
           ((uint32_t)(month / 10) << 12) |
           ((uint32_t)(month % 10) << 8)  |
           ((uint32_t)(day / 10)   << 4)  |
           ((uint32_t)(day % 10));
    /* clang-format on */
}

int dr2ymdw(const uint32_t dr,  //
            uint8_t* year,      //
            uint8_t* month,     //
            uint8_t* day,       //
            uint8_t* weekday    //
) {
    uint8_t yy = (uint8_t)(dr >> 16) & 0xFF;
    uint8_t mm = (uint8_t)(dr >> 8) & 0x1F;
    uint8_t dd = (uint8_t)(dr) & 0x3F;

    *weekday = (uint8_t)((dr >> 13) & 0x07);

    *year = bcd_to_bin(yy);
    *month = bcd_to_bin(mm);
    *day = bcd_to_bin(dd);

    return 0;
}

/*
 * STM32 RTC weekday:
 *
 * 1 = Monday
 * 2 = Tuesday
 * 3 = Wednesday
 * 4 = Thursday
 * 5 = Friday
 * 6 = Saturday
 * 7 = Sunday
 */
uint8_t rtc_weekday(uint16_t year, uint8_t month, uint8_t day) {
    static const uint8_t table[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    if (month < 3) {
        --year;
    }

    uint8_t w =
        (uint8_t)((year + (year / 4) - (year / 100) + (year / 400) + table[month - 1] + day) % 7);

    // Algorithm: 0 = Sunday, 1 = Monday, ...
    // STM32:     1 = Monday, ..., 7 = Sunday
    return (w == 0) ? 7 : w;
}

int trim4timeDate(char* buf, int* len) {
    int cn = 0;
    char ch = 0;

    for (int c = 0; buf[c] != '\0'; c++) {
        ch = buf[c];

        if (ch == '\r' || ch == '\n' || ch == '\0') {
            break;
        }

        if (ch == ' ' || ch == '=' || ch == ':' ||  //
            ch == ',' || ch == ';' || ch == '/')    //
        {
            continue;
        }

        if (ch < '0' || ch > '9') {
            continue;
        }

        buf[cn++] = ch;
    }

    buf[cn] = '\0';
    *len = cn;

    if (cn < 6) {
        return -1;
    }

    return 0;
}

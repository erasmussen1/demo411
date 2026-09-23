#pragma once

#include <stdint.h>

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} rtc_time_t;

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
} rtc_date_t;


void rtc_init(void);

int rtc_set_time(const rtc_time_t* rtc_time);
int rtc_set_date(const rtc_date_t* rtc_date);

rtc_time_t rtc_get_time(void);
rtc_date_t rtc_get_date(void);

int isTimeValid(const rtc_time_t* rtc_time);
int isDateValid(const rtc_date_t* rtc_date);

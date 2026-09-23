
#include "rtc.h"

#include "reg_rcc.h"
#include "reg_rtc.h"
#include "reg_pwr.h"

#include <utils.h>


/* clang-format off */

/* BitMasks */
#define PWR_CR_DBP           (1UL << 8U)

#define RCC_APB1ENR_PWREN    (1UL << 28U)
#define RCC_CSR_LSION        (1UL << 0U)
#define RCC_CSR_LSIRDY       (1UL << 1U)
#define RCC_BDCR_RTCSEL_MASK (3UL << 8U)
#define RCC_BDCR_RTCSEL_LSI  (2UL << 8U)
#define RCC_BDCR_RTCEN       (1UL << 15U)

#define RTC_ISR_INITF        (1UL << 6U)
#define RTC_ISR_INIT         (1UL << 7U)
#define RTC_CR_FMT           (1UL << 6U)
/* clang-format on */

/* INITF normally becomes set after two RTC clock cycles.
 * Keep the wait bounded so a stopped RTC clock cannot
 * trap a calling FreeRTOS task.
 */
#define RTC_INIT_TIMEOUT 100000UL


void rtc_init(void) {
    /* 1. Enable power interface clock. */
    RCC->apb1enr |= RCC_APB1ENR_PWREN;

    /* 2. Allow access to the backup domain.  */
    PWR->cr |= PWR_CR_DBP;

    /* 3. Enable LSI oscillator.  */
    RCC->csr |= RCC_CSR_LSION;

    while ((RCC->csr & RCC_CSR_LSIRDY) == 0UL) {
    }

    /*
     * The backup registers and RTC configuration survive a system reset, but
     * LSION does not.  Always restore the RTC clock before using the backup
     * marker to decide whether the calendar itself needs configuring.
     */
    if (RTC->bkp0r == 0x12345677) {
        RCC->bdcr |= RCC_BDCR_RTCEN;
        return;
    }

    /*
     * 4. Select LSI as RTC clock source.
     * RTCSEL:
     *   00 = no clock
     *   01 = LSE
     *   10 = LSI
     *   11 = HSE / 32
     */
    RCC->bdcr = (RCC->bdcr & ~RCC_BDCR_RTCSEL_MASK) | RCC_BDCR_RTCSEL_LSI;

    /* 5. Enable RTC.  */
    RCC->bdcr |= RCC_BDCR_RTCEN;

    /* 6. Disable RTC write protection.  */
    RTC->wpr = 0xCAUL;
    RTC->wpr = 0x53UL;

    /* 7. Enter RTC initialization mode.  */
    RTC->isr |= RTC_ISR_INIT;

    while ((RTC->isr & RTC_ISR_INITF) == 0UL) {
    }

    /*
     * LSI is approximately 32 kHz.
     * RTC clock:
     *   32000 / ((127 + 1) * (249 + 1)) = 1 Hz
     *
     * PREDIV_A = 127
     * PREDIV_S = 249
     */
    RTC->prer = (127UL << 16U) | 249UL;

    /* Use 24-hour format. */
    RTC->cr &= ~RTC_CR_FMT;

    /* Leave initialization mode.  */
    RTC->isr &= ~RTC_ISR_INIT;

    RTC->bkp0r = 0x12345677;

    /* Re-enable write protection.  */
    RTC->wpr = 0xFFUL;
}

static int rtc_set_dt(const uint8_t setTime, const uint32_t regValue) {
    int rc = 0;
    RCC->apb1enr |= RCC_APB1ENR_PWREN;
    PWR->cr |= PWR_CR_DBP;

    RTC->wpr = 0xCAUL;
    RTC->wpr = 0x53UL;

    RTC->isr |= RTC_ISR_INIT;

    uint32_t i = 0;
    while ((RTC->isr & RTC_ISR_INITF) == 0UL) {
        if (i++ < RTC_INIT_TIMEOUT) {
            continue;
        }

        rc = -1;
        break;
    }

    if (rc == 0) {
        if (setTime) {
            RTC->tr = regValue;
        } else {
            RTC->dr = regValue;
        }
    }

    RTC->isr &= ~RTC_ISR_INIT;
    RTC->wpr = 0xFFUL;

    return rc;
}

rtc_time_t rtc_get_time(void) {
    uint32_t tr = RTC->tr;

    /* Reading DR unlocks the RTC shadow registers after reading TR. */
    uint32_t dr = RTC->dr;

    uint8_t hour = (uint8_t)((tr >> 16) & 0x3F);
    uint8_t minute = (uint8_t)((tr >> 8) & 0x7F);
    uint8_t second = (uint8_t)(tr & 0x7F);

    rtc_time_t time;
    time.hour = bcd_to_bin(hour);
    time.minute = bcd_to_bin(minute);
    time.second = bcd_to_bin(second);

    return time;
}

rtc_date_t rtc_get_date(void) {
    (void)RTC->tr;

    /* Reading DR unlocks the RTC shadow registers after reading TR. */
    uint32_t dr = RTC->dr;

    rtc_date_t date;
    uint8_t weekday = 0;
    (void)dr2ymdw(dr, &date.year, &date.month, &date.day, &weekday);

    return date;
}

int rtc_set_time(const rtc_time_t* t) {
    const uint8_t h = bin_to_bcd(t->hour);
    const uint8_t m = bin_to_bcd(t->minute);
    const uint8_t s = bin_to_bcd(t->second);

    const uint32_t tr = (uint32_t)(h & 0x3F) << 16 |  //
        (uint32_t)(m & 0x7F) << 8 |                   //
        (uint32_t)(s & 0x7F);

    return rtc_set_dt(1, tr);
}

int rtc_set_date(const rtc_date_t* d) {
    const uint16_t yearReal = 2000 + d->year;

    const uint8_t weekday = rtc_weekday(yearReal, d->month, d->day);

    const uint32_t dr = ymdw2dr(d->year, d->month, d->day, weekday);

    return rtc_set_dt(0, dr);
}

int isTimeValid(const rtc_time_t* t) {
    return (t->hour > 59U || t->minute > 59U || t->second > 59U) ? -1 : 0;
}

int isDateValid(const rtc_date_t* d) {
    return (d->year > 45U || d->month > 12U || d->day > 31U) ? -1 : 0;
}

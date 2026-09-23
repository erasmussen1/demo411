
#include "amenu.h"
#include "menu_types.h"
#include "menu_util.h"

#include <stdint.h>
#include <stddef.h>


/* Prototypes.*/
static void AM_set_prime1(char* cmdbuf);
static void AM_set_prime2(char* cmdbuf);
static void AM_set_prime3(char* cmdbuf);
static void AM_set_delay(char* cmdbuf);
static void AM_set_mask(char* cmdbuf);

AM_values_t AM_values;

void AM_initMenu(void) {
    AM_values.prime1 = 41;
    AM_values.prime2 = 43;
    AM_values.prime3 = 47;
    AM_values.delay = 250;
    AM_values.mask = 0x8001;
}

AM_values_t* AM_getData(void) {
    return &AM_values;
}

/* clang-format off */
ItemList AM_prime_number_1[] = {
    {ITEM_SHORT, (void*)(&AM_values.prime1), 0, 0, 3, "%03d"},
    {ITEM_LAST, NULL, 0, 0, 0, NULL}
};

ItemList AM_prime_number_2[] = {
	{ITEM_SHORT, (void*)(&AM_values.prime2), 0, 0, 3, "%03d"},
	{ITEM_LAST, NULL, 0, 0, 0, NULL}
};

ItemList AM_mask[] = {
    {ITEM_SHORT, (void*)(&AM_values.mask), 0x0080, 7, 1, "%1d"},
    {ITEM_SHORT, (void*)(&AM_values.mask), 0x0040, 6, 1, "%1d"},
    {ITEM_SHORT, (void*)(&AM_values.mask), 0x0020, 5, 1, "%1d"},
    {ITEM_SHORT, (void*)(&AM_values.mask), 0x0010, 4, 1, "%1d"},
    {ITEM_SHORT, (void*)(&AM_values.mask), 0x0008, 3, 1, "%1d"},
    {ITEM_SHORT, (void*)(&AM_values.mask), 0x0004, 2, 1, "%1d"},
    {ITEM_SHORT, (void*)(&AM_values.mask), 0x0002, 1, 1, "%1d"},
    {ITEM_SHORT, (void*)(&AM_values.mask), 0x0001, 0, 1, "%1d"},
    {ITEM_LAST, NULL, 0, 0, 0, NULL}
};

ItemList AM_delay[] = {
    {ITEM_SHORT, (void*)(&AM_values.delay), 0, 0, 3, "%03d"},
    {ITEM_LAST, NULL, 0, 0, 0, NULL}
};

/* clang-format on */

static void AM_set_prime1(char* cmdbuf) {
    UTIL_get_value(&AM_values.prime1, 0, 600, cmdbuf);
}

static void AM_set_prime2(char* cmdbuf) {
    UTIL_get_value(&AM_values.prime2, 0, 600, cmdbuf);
}

static void AM_set_prime3(char* cmdbuf) {
    UTIL_get_value(&AM_values.prime3, 0, 600, cmdbuf);
}

static void AM_set_delay(char* cmdbuf) {
    UTIL_get_value(&AM_values.delay, 0, 65535, cmdbuf);
}

static void AM_set_mask(char* cmdbuf) {
    unsigned long value;
    int rc = UTIL_get_mask(cmdbuf, 8, &value);
    if (!rc) {
        AM_values.mask = (unsigned short)value;
    }
}

/* clang-format off */
const Cmd AM_menu_table[] = {
    {"Z", 0, AM_set_prime1, "1st Prime Number", AM_prime_number_1, NULL },
    {"E", 0, AM_set_prime2, "2nd Prime Number", AM_prime_number_2 , NULL },
    {"X", 0, AM_set_mask,   "Control mask", AM_mask              , NULL  },
    {"T", 0, AM_set_delay,  "Delay between LED on (mSec)", AM_delay , NULL },
    {"?", 0, 0, "Display help", NULL, NULL},
    { 0,  0, 0, 0, NULL, NULL}
};
/* clang-format on */


#include "bmenu.h"
#include "menu.h"
#include "menu_util.h"

#include <stdint.h>
#include <stddef.h>


/* Prototypes.*/
static void BM_set_prime1(char* cmdbuf);
static void BM_set_prime2(char* cmdbuf);
static void BM_set_prime3(char* cmdbuf);
static void BM_set_delay(char* cmdbuf);

BM_values_t BM_values;

void BM_initMenu(void) {
    BM_values.prime1 = 41;
    BM_values.prime2 = 43;
    BM_values.prime3 = 47;
    BM_values.delay = 250;
}

BM_values_t* BM_getData(void) {
    return &BM_values;
}

/* clang-format off */

ItemList BM_prime_number_1[] = {
    {ITEM_SHORT, (void*)(&BM_values.prime1), 0, 0, 3, "%03d"},
    {ITEM_LAST, NULL, 0, 0, 0, NULL}
};

ItemList BM_prime_number_2[] = {
    {ITEM_SHORT, (void*)(&BM_values.prime2), 0, 0, 3, "%03d"},
    {ITEM_LAST, NULL, 0, 0, 0, NULL}
};

ItemList BM_prime_number_3[] = {
    {ITEM_SHORT, (void*)(&BM_values.prime3), 0, 0, 3, "%03d"},
    {ITEM_LAST, NULL, 0, 0, 0, NULL}
};

ItemList BM_delay[] = {
    {ITEM_SHORT, (void*)(&BM_values.delay), 0, 0, 3, "%03d"},
    {ITEM_LAST, NULL, 0, 0, 0, NULL}
};

/* clang-format on */

static void BM_set_prime1(char* cmdbuf) {
    UTIL_get_value(&BM_values.prime1, 0, 600, cmdbuf);
}

static void BM_set_prime2(char* cmdbuf) {
    UTIL_get_value(&BM_values.prime2, 0, 600, cmdbuf);
}

static void BM_set_prime3(char* cmdbuf) {
    UTIL_get_value(&BM_values.prime3, 0, 600, cmdbuf);
}

static void BM_set_delay(char* cmdbuf) {
    UTIL_get_value(&BM_values.delay, 0, 65535, cmdbuf);
}

/* clang-format off */
const Cmd BM_menu_table[] = {
    {"A", 0, BM_set_prime1, "Value 1", BM_prime_number_1, NULL},
    {"B", 0, BM_set_prime2, "Value 2", BM_prime_number_2, NULL},
    {"C", 0, BM_set_prime3, "Value 3", BM_prime_number_3, NULL},
    {"T", 0, BM_set_delay, "Delay between LED on (mSec)", BM_delay, NULL},
    {"?", 0, 0, "Display help", NULL, NULL},
    {0, 0, 0, 0, NULL, NULL}
};
/* clang-format on */

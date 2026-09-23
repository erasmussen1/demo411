#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned short prime1;
    unsigned short prime2;
    unsigned short prime3;
    unsigned short delay;
    unsigned short mask;
} AM_values_t;

void AM_initMenu(void);

AM_values_t* AM_getData(void);

#ifdef __cplusplus
}
#endif

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned short delay;
    unsigned short mask;
} TM_values_t;

void TM_initMenu(void);

TM_values_t* TM_getData(void);

#ifdef __cplusplus
}
#endif

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned short prime1;
    unsigned short prime2;
    unsigned short prime3;
    unsigned short delay;
} BM_values_t;

void BM_initMenu(void);

BM_values_t* BM_getData(void);

#ifdef __cplusplus
}
#endif

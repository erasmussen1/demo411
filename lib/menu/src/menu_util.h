#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void UTIL_get_value(unsigned short* var,  //
                    unsigned short min,   //
                    unsigned short max,   //
                    char* cmdbuf);

int UTIL_get_mask(char* cmdBuf,                       //
                  const unsigned short numberOfBits,  //
                  unsigned long* value);

#ifdef __cplusplus
}
#endif

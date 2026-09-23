#pragma once

#include <menu_types.h>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void ME_initCommandBuffer(char* cmdBuffer,  //
                          const int cmdBufferSize);

int ME_command(char* cmdBuffer,              //
               const int cmdBufferSize,      //
               void (*delay_fn)(const int),  //
               const int tick);

int ME_commandProcess(char* cmdBuffer,  //
                      const int commandLength);

void ME_commandError(long errcode, char* errstr);


#ifdef __cplusplus
}
#endif

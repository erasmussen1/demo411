

#include "menu_util.h"

#include <stdio.h>


void UTIL_get_value(unsigned short* var,  //
                    unsigned short min,   //
                    unsigned short max,   //
                    char* cmdBuf          //
) {
    char* arg = &cmdBuf[2];
    int temp = 0;
    int rc = sscanf(arg, "%d", &temp);
    if (rc != 1) {
        return;
    }

    if ((temp > (int)max) || (temp < (int)min)) {
        return;
    }

    *var = (unsigned short)temp;
}

int UTIL_get_mask(char* cmdBuf,                       //
                  const unsigned short numberOfBits,  //
                  unsigned long* value                //
) {
    char* arg = &cmdBuf[2];

    unsigned long tmp = 0;
    unsigned short bits = 0;
    for (unsigned short i = 0; arg[i] != '\0'; ++i) {
        if (i >= numberOfBits) {
            break;
        }

        if (arg[i] != '0' && arg[i] != '1') {
            continue;
        }

        tmp = (tmp << 1) | (unsigned long)(arg[i] - '0');
        bits++;
    }

    if (bits != numberOfBits) {
        return -1;
    }

    *value = tmp;

    return 0;
}

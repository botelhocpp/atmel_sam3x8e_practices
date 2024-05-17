#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

#define FTOA_MAX_PRECISION	    (5)

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

char *itoa(uint32_t value, char *str, uint32_t radix);

char *ftoa(float f, char *str, int precision);

#endif

#ifndef HELPER
#define HELPER

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#include <stdbool.h>
#include "uart.h"

void busyWait(uint32_t ticks);
uint16_t random(uint16_t range, uint16_t start);
double toRad(int angle);

#endif

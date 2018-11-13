#ifndef HELPER
#define HELPER

#include <stdbool.h>
#include "uart.h"

void busyWait(uint32_t ticks);
uint16_t random(uint16_t range, uint16_t start);

#endif
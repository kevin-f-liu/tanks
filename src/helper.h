#ifndef HELPER
#define HELPER

#include <stdbool.h>
#include "coordinate.h"
#include "uart.h"

void busyWait(uint32_t ticks);
uint16_t random(uint16_t range, uint16_t start);
int16_t processValue(int16_t val, int16_t upperbound, int16_t lowerbound);
double dist(Coordinate *c1, Coordinate *c2);

#endif

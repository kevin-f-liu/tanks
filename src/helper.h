#ifndef HELPER
#define HELPER

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#include <stdbool.h>
#include "coordinate.h"
#include "uart.h"

void busyWait();
uint16_t random(uint16_t range, uint16_t start);
int16_t processValue(int16_t val, int16_t upperbound, int16_t lowerbound);
double dist(Coordinate *c1, Coordinate *c2);
double toRad(int angle);

#endif

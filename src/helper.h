#ifndef HELPER
#define HELPER

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <stdbool.h>
#include "coordinate.h"
#include "uart.h"

void busyWait(uint32_t ticks);
int32_t random(int32_t lower, int32_t upper);
int16_t processValue(int16_t val, int16_t upperbound, int16_t lowerbound);
double dist(Coordinate *c1, Coordinate *c2);
double toRad(int angle);
float fastInvsqrt(float number);
bool isEdge(Coordinate c, uint16_t width, uint16_t height);
int min(int a, int b);
int max(int a, int b);
#endif

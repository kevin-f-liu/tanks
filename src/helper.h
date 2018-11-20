#ifndef HELPER
#define HELPER

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "coordinate.h"
#include "uart.h"
#include <stdbool.h>

// for loop with ticks as counter
void busyWait(uint32_t ticks);
// generate a random number within range
int32_t random(int32_t lower, int32_t upper);
// check if val is within bounds. If not, return closest value within bounds
int16_t processValue(int16_t val, int16_t upperbound, int16_t lowerbound);
// returns euclidean distance between coordinates
double dist(Coordinate *c1, Coordinate *c2);
// convert deg to rad
double toRad(int angle);
float fastInvsqrt(float number);
bool isEdge(Coordinate c, uint16_t width, uint16_t height);
int min(int a, int b);
int max(int a, int b);
#endif

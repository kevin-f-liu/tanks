#include "helper.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

void busyWait(uint32_t ticks) {
  for (int i = 0; i < ticks; i++)
    ;
}

int32_t random(int32_t lower, int32_t upper) {
  int32_t x = rand() % (upper - lower + 1) + lower;
  return x;
}

int16_t processValue(int16_t val, int16_t upperbound, int16_t lowerbound) {
  if (val >= upperbound) return upperbound;
  if (val <= lowerbound) return lowerbound;
  return val;
}

bool isEdge(Coordinate c, uint16_t width, uint16_t height) {
	return c.x == 0 || c.y == 0 || c.x == width-1 || c.y == height-1;
}

float fastInvsqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;      
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration

	return y;
}

double dist(Coordinate *c1, Coordinate *c2) {
  return sqrt((c1->x - c2->x) * (c1->x - c2->x) + (c1->y - c2->y) * (c1->y - c2->y));
}

inline double toRad(int angle) {
	return angle * M_PI / 180;
}

int min(int a, int b) { return a <= b ? a : b; }

int max(int a, int b) { return a >= b ? a : b; }




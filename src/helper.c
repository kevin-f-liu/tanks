#include "helper.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

void busyWait(uint32_t ticks) {
  for (int i = 0; i < ticks; i++)
    ;
}

uint16_t random(uint16_t lower, uint16_t upper) {
  uint16_t x = rand() % (upper - lower + 1) + lower;
  return x;
}

inline double toRad(int angle) {
	return angle * M_PI / 180;
}
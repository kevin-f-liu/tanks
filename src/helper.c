#include "helper.h"
#include <stdlib.h>
#include <time.h>

void busyWait(uint32_t ticks) {
  for (int i = 0; i < ticks; i++)
    ;
}

uint16_t random(uint16_t lower, uint16_t upper) {
  uint16_t x = rand() % (upper - lower + 1) + lower;
  return x;
}

int16_t processValue(int16_t val, int16_t upperbound, int16_t lowerbound) {
  if (val >= upperbound) return upperbound;
  if (val <= lowerbound) return lowerbound;
  return val;
}
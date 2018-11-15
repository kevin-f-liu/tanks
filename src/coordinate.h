#ifndef COORDINATE
#define COORDINATE

#include "uart.h"

typedef struct {
  // between 0 to 319
  uint16_t x;
  // between 0 to 239
  uint16_t y;
} Coordinate;

void updateCoordinate(Coordinate *c, uint16_t newX, uint16_t newY);

#endif

#ifndef COORDINATE
#define COORDINATE

#include "uart.h"

typedef struct {
  // between 1 to 320
  uint16_t x;
  // between 1 to 240
  uint16_t y;
} Coordinate;

void updateCoordinate(Coordinate *c, uint16_t newX, uint16_t newY);

#endif

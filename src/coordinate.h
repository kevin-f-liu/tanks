#ifndef COORDINATE
#define COORDINATE

#include "uart.h"

#define BOUNDARY 4
typedef struct {
  // between 0 to 319
  int x;
  // between 0 to 239
  int y;
} Coordinate;

void updateCoordinate(Coordinate *c, uint16_t newX, uint16_t newY);

#endif

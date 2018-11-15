#include "coordinate.h"
#include "helper.h"
#include "terrain.h"

void updateCoordinate(Coordinate *c, uint16_t newX, uint16_t newY) {
  c->x = processValue(newX, TERRAIN_WIDTH - 1, 0);
  c->y = processValue(newY, TERRAIN_HEIGHT - 1, 0);
}
#include "player.h"
#include "helper.h"

void updateHealth(Player *p, Coordinate land) {}

void updatePosition(Player *p, int16_t newX, Terrain terrain) {
  updateCoordinate(&p->pos, newX, closestY(terrain, p->pos));
}

void updateAim(Player *p, int16_t newAim) {
  p->aimAngle = processValue(newAim, 180, 0);
}

#include "player.h"

void updateHealth(Player *p, uint16_t landX, uint16_t landY) {}

int16_t processValue(int16_t val, int16_t upperbound, int16_t lowerbound) {
  if (val >= upperbound) return upperbound;
  if (val <= lowerbound) return lowerbound;
  return val;
}

void updatePosition(Player *p, int16_t newX, Terrain terrain) {
  p->x = processValue(newX, 319, 0);
  p->y = closestY(terrain, p->x, p->y);
}

void updateAim(Player *p, int16_t newAim) {
  p->aimAngle = processValue(newAim, 180, 0);
}

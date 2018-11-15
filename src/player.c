#include "player.h"
#include "helper.h"

void updateHealth(Player *p, Coordinate *land) {
  double d = dist(&p->pos, land);
  // reduce hp if the tank is within the hit radius
  if (d < RADIUS_OF_DAMAGE) {
    p->HP -= MAX_DAMAGE - d / RADIUS_OF_DAMAGE * MAX_DAMAGE;
  }
}

void updatePosition(Player *p, int16_t dx, Terrain *terrain) {
  int16_t newX = processValue(p->pos.x + dx, TERRAIN_WIDTH - 1, 0);
  updateCoordinate(&p->pos, newX, closestGround(terrain, newX, p->pos.y));
}

void updateAim(Player *p, int16_t newAim) {
  p->aimAngle = processValue(newAim, 180, 0);
}

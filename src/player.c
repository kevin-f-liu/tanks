#include "player.h"
#include <math.h>
#include <stdio.h>
#include "helper.h"

void setupPlayer(Player *p, bool isP1) {
  p->HP = 100;
  p->pos.y = 0;
  p->aimAngle = isP1 ? 0 : 180;
}

void updateHealth(Player *p, Coordinate *land) {
  double d = dist(&p->pos, land);
		printf("d: (%f)\n", d);
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

void printPlayer(Player *p) {
  printf("\n");
  printf("HP: %d\n", p->HP);
  printf("Pos: (%d,%d)\n", p->pos.x, p->pos.y);
  printf("Aim: %d\n", p->aimAngle);
}

void updateStatus(Player *p, Terrain *terrain, Coordinate *ball) {
  updateHealth(p, ball);
  updatePosition(p, 0, terrain);
  printPlayer(p);
}

bool fire(Player *p, Coordinate *ball, Terrain *terrain, uint16_t firepower) {
  int32_t dx = round(firepower * cos(toRad(p->aimAngle)));
  int32_t dy = round(firepower * sin(toRad(p->aimAngle)));
  bool passPeak = false;
  // move until collison or x out of range or y too low
  while (!collide(terrain, ball)) {
    // out of range
    if (ball->x + dx > TERRAIN_WIDTH || ball->x + dx < 0 || ball->y - dy > TERRAIN_HEIGHT ) return false;
    ball->x += dx;
		ball->y -= dy;
		
		printf("Ball: (%d,%d)\n", ball->x, ball->y);
    if (firepower == 0) {
      passPeak = true;
    }
    dy--;
  }
  return true;
}
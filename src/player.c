#include "player.h"
#include <math.h>
#include <stdio.h>
#include "helper.h"

void setupPlayer(Player *p) {
  p->HP = 100;
  p->pos.y = 0;
  p->aimAngle = 0;
}

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
  int16_t dx = 0;
  bool isHorizontal_180 = p->aimAngle == 180;
  bool isHorizontal_0 = p->aimAngle == 0;
  bool isVertical = p->aimAngle == 90;
  // if not veritcal or horizontal
  if (!isHorizontal_180 && !isHorizontal_0 && !isVertical) {
    dx = firepower / tan(p->aimAngle * acos(-1.0) / 180);
    if (p->aimAngle > 90) dx = -dx;
  }
  bool passPeak = false;
  // move until collison or x out of range cuz y is always gonna come back down
  while (!collide(terrain, ball)) {
    int16_t changeX = 0;
    int16_t changeY = 0;
    if (isHorizontal_180) {
      changeX = -firepower;
      changeY = 1;
    } else if (isHorizontal_0) {
      changeX = firepower;
      changeY = 1;
    } else if (isVertical) {
      changeY = firepower;
    } else {
      changeX = dx;
      changeY = passPeak ? firepower : -firepower;
    }
    // out of range
    if (ball->x + changeX > TERRAIN_WIDTH || ball->x + changeX < 0)
      return false;
    updateCoordinate(ball, ball->x + changeX, ball->y + changeY);
    if (firepower == 0) {
      passPeak = true;
    }
    firepower = passPeak ? firepower + 1 : firepower - 1;
  }
  return true;
}
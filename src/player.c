#include "player.h"
#include <math.h>
#include <stdio.h>
#include "graphics.h"
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
  int16_t newX = processValue(p->pos.x + dx, TERRAIN_WIDTH - 1 - BOUNDARY, BOUNDARY);
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
  int32_t tempX = ball->x;
  int32_t tempY = ball->y;
  // move until collison or x out of range or y too low
  while (!collide(terrain, ball)) {
    tempX += dx;
    tempY -= dy;
    // out of range
    if (tempX > TERRAIN_WIDTH || tempX < 0) return false;
    // no more ground to hit
    if (tempY >= TERRAIN_HEIGHT) {
      // explode at edge of screen
      ball->x = tempX;
      ball->y = TERRAIN_HEIGHT - 1;
      return true;
    }

    // when ball is supposedly outside the screen, hide it
    if (tempY < 0) {
      *ball = p->pos;
    }
    // when ball is inside the screen, that's when collision will happen
    else {
      ball->x = tempX;
      ball->y = tempY;
    }

    printf("Ball: (%d,%d)\n", ball->x, ball->y);
    printf("TempBall: (%d,%d)\n", tempX, tempY);
    dy--;
    // leave time for graphics to update
    busyWait(10000);
  }
  return true;
}

void updateGraphics(Player *p, bool isP1) {
  uint8_t playerNum = isP1 ? 1 : 2;
  moveTank(p->pos, playerNum);
  aimTank(p->aimAngle, playerNum);
  updateHealthBar(p->HP, playerNum);
}

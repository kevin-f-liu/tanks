#include "player.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "helper.h"

void setupPlayer(Player *p, bool isP1) {
  p->HP = 100;
  p->pos.y = 0;
  p->aimAngle = isP1 ? 0 : 180;
}

void updateHealth(Player *p, Coordinate *land) {
  double d = dist(&p->pos, land);
  // reduce hp if the tank is within the hit radius
  if (d < RADIUS_OF_DAMAGE) {
    p->HP -= MAX_DAMAGE - d / RADIUS_OF_DAMAGE * MAX_DAMAGE;
  }
}

bool isOverlap(Player *p1, Player *p2) {
  return abs(p1->pos.x - p2->pos.x) < TANK_WIDTH_COORD + 2 &&
         abs(p1->pos.y - p2->pos.y) < TANK_HEIGHT_COORD;
}

void updatePositionWithCheck(Player *p, int16_t dx, Terrain *terrain,
                             Player *p2) {
  if (dx == 0) return;
  Player tempPlayer = *p;
  updatePosition(&tempPlayer, dx, terrain);
  if (!isOverlap(&tempPlayer, p2)) {
    *p = tempPlayer;
  }
}

void updatePosition(Player *p, int16_t dx, Terrain *terrain) {
  uint16_t newX =
      processValue(p->pos.x + dx, TERRAIN_WIDTH - 1 - TANK_WIDTH_COORD / 2,
                   TANK_WIDTH_COORD / 2);
  uint32_t newY = closestGround(terrain, newX, p->pos.y);
  int32_t ceil = ceiling(terrain, p->pos.x, p->pos.y);
  if (ceil >= 0 && ceil >= newY) return;
  updateCoordinate(&p->pos, newX, newY);
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

double interval(int32_t self, int32_t other) {
  if (self == 0) return 0;
  // if i am smaller than the other, use me as the base
  if (abs(self) < abs(other) || other == 0) {
    if (self < 0) return -1;
    return 1;
  }
  return (double)self / abs(other);
}

bool hitTank(Player *p, Coordinate *ball) {
  return (ball->x <= p->pos.x + TANK_WIDTH_COORD / 2) &&
         (ball->x >= p->pos.x - TANK_WIDTH_COORD / 2) &&
         (ball->y <= p->pos.y) && (ball->y >= p->pos.y - TANK_HEIGHT_COORD);
}

bool fire(Player *p, Player *p2, Coordinate *ball, Terrain *terrain,
          uint16_t firepower) {
  int32_t dx = round(firepower / 10.0 * cos(toRad(p->aimAngle)));
  int32_t dy = round(firepower / 10.0 * sin(toRad(p->aimAngle)));
  bool xIsBase = abs(dx) < abs(dy) || dy == 0;
  *ball = p->pos;
  ball->y -= 3;
  Coordinate tempBall = *ball;
  int32_t tempX = tempBall.x;
  int32_t tempY = tempBall.y;
  double y_int = interval(dy, dx);
  double x_int = interval(dx, dy);
  uint32_t count = 1;

  // move until collison or x out of range or y too low
  while (!collide(terrain, ball) && !hitTank(p2, ball)) {
    tempX = tempBall.x + round(x_int * count);
    tempY = tempBall.y - round(y_int * count);
    // out of range
    if (tempX > TERRAIN_WIDTH || tempX < 0) return false;
    // no more ground to hit
    if (tempY >= TERRAIN_HEIGHT) {
      // explode at edge of screen
      updateCoordinate(ball, tempX, TERRAIN_HEIGHT - 1);
      return true;
    }

    // when ball is supposedly outside the screen, hide it
    if (tempY < 0) {
      hideShot(ball);
    }
    // when ball is inside the screen, that's when collision will happen
    else {
      updateCoordinate(ball, tempX, tempY);
    }

    if ((xIsBase && count == abs(dx)) || count == abs(dy) ||
        (dy == 0 && dx == 0)) {
      dy--;
      tempBall.x = tempX;
      tempBall.y = tempY;
      xIsBase = abs(dx) < abs(dy) || dy == 0;
      y_int = interval(dy, dx);
      x_int = interval(dx, dy);
      count = 0;
      busyWait(300000);
    }
    // leave time for graphics to update
    // printf("%d,%d\n",tempX,tempY);
    count++;
  }
  return true;
}

void updateGraphics(Player *p, bool isP1) {
  uint8_t playerNum = isP1 ? 1 : 2;
  moveTank(p->pos, playerNum);
  aimTank(p->aimAngle, playerNum);
  updateHealthBar(p->HP, playerNum);
}

void hideShot(Coordinate *ball) {
  updateCoordinate(ball, 0, TERRAIN_HEIGHT - 1);
}

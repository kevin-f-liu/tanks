#include "player.h"
#include <math.h>
#include <stdio.h>
#include "graphics.h"
#include "helper.h"
#include <stdlib.h>

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

void updatePosition(Player *p, int16_t dx, Terrain *terrain) {
  uint16_t newX = processValue(p->pos.x + dx, TERRAIN_WIDTH - 1 - BOUNDARY, BOUNDARY);
	uint32_t newY = closestGround(terrain, newX, p->pos.y);
	int32_t ceil = ceiling(terrain, p->pos.x, p->pos.y);
	if (ceil >= 0 && ceil >= newY)  return;
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

double interval(int32_t self, int32_t other){
	if (self == 0) return 0;
	// if i am smaller than the other, use me as the base
	if (abs(self) < abs(other) || other == 0){
		if (self < 0) return -1;
		return 1;
	}			
	return (double)self/abs(other);
}

bool fire(Player *p, Coordinate *ball, Terrain *terrain, uint16_t firepower) {
  int32_t dx = round(firepower/10.0 * cos(toRad(p->aimAngle)));
  int32_t dy = round(firepower/10.0 * sin(toRad(p->aimAngle)));
	bool xIsBase = abs(dx) < abs(dy) || dy == 0;
	*ball = p->pos;
	ball->y -= 3;
	Coordinate tempBall = *ball;
  int32_t tempX = tempBall.x;
  int32_t tempY = tempBall.y;
	double y_int = interval(dy,dx);
	double x_int = interval(dx,dy);
	uint32_t count = 1;
	
  // move until collison or x out of range or y too low
  while (!collide(terrain, ball)) {
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

		if ((xIsBase && count == abs(dx)) || count == abs(dy)|| (dy == 0 && dx == 0)){
			dy--;
			tempBall.x = tempX;
			tempBall.y = tempY;
			xIsBase = abs(dx) < abs(dy) || dy == 0;
			y_int = interval(dy,dx);
			x_int = interval(dx,dy);
			count = 0;
			busyWait(100000);
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

void hideShot(Coordinate *ball){
	updateCoordinate(ball,0,TERRAIN_HEIGHT-1);
}


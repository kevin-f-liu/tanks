#ifndef PLAYER
#define PLAYER

#include "coordinate.h"
#include "terrain.h"
#include "uart.h"

#define MAX_FIREPOWER 100
#define RADIUS_OF_DAMAGE 5
#define MAX_DAMAGE 90
#define BOUNDARY 4

typedef struct {
  // HP max 100
  int8_t HP;
  Coordinate pos;
  //-179 to 180
  int16_t aimAngle;
} Player;

void setupPlayer(Player *p, bool isP1);
void updateHealth(Player *p, Coordinate *land);
void updatePosition(Player *p, int16_t newX, Terrain *terrain);
void updateAim(Player *p, int16_t newAim);
void printPlayer(Player *p);
void updateStatus(Player *p, Terrain *terrain, Coordinate *ball);
// return true at collison, false if out of range
bool fire(Player *p, Coordinate *ball, Terrain *terrain, uint16_t firepower);
void updateGraphics(Player *p, bool isP1);
void hideShot(Coordinate *ball);

#endif

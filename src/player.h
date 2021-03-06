#ifndef PLAYER
#define PLAYER

#include "coordinate.h"
#include "sprites.h"
#include "terrain.h"
#include "uart.h"

#define MAX_FIREPOWER 100
#define RADIUS_OF_DAMAGE 5
#define MAX_DAMAGE 90
#define TANK_WIDTH_COORD TANK_WIDTH / 4
#define TANK_HEIGHT_COORD TANK_HEIGHT / 4

typedef struct {
  // HP max 100
  int8_t HP;
  Coordinate pos;
  //-179 to 180
  int16_t aimAngle;
} Player;

// setup initial status of player
void setupPlayer(Player *p, bool isP1);
void updateHealth(Player *p, Coordinate *land);
// check for overlap before updating position
void updatePositionWithCheck(Player *p, int16_t dx, Terrain *terrain, Player *p2);
void updatePosition(Player *p, int16_t newX, Terrain *terrain);
void updateAim(Player *p, int16_t newAim);
// prints player info
void printPlayer(Player *p);
// update player health and position based on the ball position
void updateStatus(Player *p, Terrain *terrain, Coordinate *ball);
// return true at collison, false if out of range, update ball coordinate in the
// process
bool fire(Player *p, Player *p2, Coordinate *ball, Terrain *terrain, uint16_t firepower);
// call graphics functions related to player
void updateGraphics(Player *p, bool isP1);
// move shot out of sight
void hideShot(Coordinate *ball);

#endif

#ifndef PLAYER
#define PLAYER

#include "coordinate.h"
#include "terrain.h"
#include "uart.h"

#define MAX_FIREPOWER 10
#define RADIUS_OF_DAMAGE 3
#define MAX_DAMAGE 20

typedef struct {
  // HP max 100
  int8_t HP;
  Coordinate pos;
  //-179 to 180
  int16_t aimAngle;
} Player;

void setupPlayer(Player *p);
void updateHealth(Player *p, Coordinate *land);
void updatePosition(Player *p, int16_t newX, Terrain *terrain);
void updateAim(Player *p, int16_t newAim);
void printPlayer(Player *p);
// void fire(Player p, uint16_t firepower);

#endif

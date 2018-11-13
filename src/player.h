#ifndef PLAYER
#define PLAYER

#include "terrain.h"
#include "uart.h"

typedef struct {
  // HP max 100
  int8_t HP;
  // between 1 to 320
  uint16_t x;
  // between 1 to 240
  uint16_t y;
  //-179 to 180
  int16_t aimAngle;
} Player;

void updateHealth(Player *p, uint16_t landX, uint16_t landY);
void updatePosition(Player *p, int16_t newX, Terrain terrain);
void updateAim(Player *p, int16_t newAim);

#endif

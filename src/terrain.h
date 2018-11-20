#ifndef TERRAIN_H
#define TERRAIN_H
#include <stdbool.h>
#include "coordinate.h"
#include "uart.h"

#define TERRAIN_WIDTH 80   // Screen width in game blocks
#define TERRAIN_HEIGHT 60  // Screen width in game blocks
#define TERRAIN_LENGTH \
  (TERRAIN_WIDTH * TERRAIN_HEIGHT)  // length of terrain array

typedef struct {
  char x[TERRAIN_LENGTH];
} Terrain;

void generateTerrain(Terrain *terrain);
void printTerrain(Terrain *terrain);
bool collide(Terrain *terrain, Coordinate *c);
void damage(Terrain *terrain, Coordinate *c);
int16_t ceiling(Terrain* terrain, uint16_t x, uint16_t oldY);
// return the closest y value below given y
uint16_t closestGround(Terrain *terrain, uint16_t newX, uint16_t oldY);
uint32_t getIndex(uint16_t x, uint16_t y);
Coordinate getCoord(uint16_t idx);

#endif

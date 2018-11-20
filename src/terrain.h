#ifndef TERRAIN_H
#define TERRAIN_H
#include "coordinate.h"
#include "uart.h"
#include <stdbool.h>

#define TERRAIN_WIDTH 80                                // Screen width in game blocks
#define TERRAIN_HEIGHT 60                               // Screen width in game blocks
#define TERRAIN_LENGTH (TERRAIN_WIDTH * TERRAIN_HEIGHT) // length of terrain array

typedef struct {
  char x[TERRAIN_LENGTH];
} Terrain;

void generateTerrain(Terrain *terrain);
void printTerrain(Terrain *terrain);
// check if c and terrain collide
bool collide(Terrain *terrain, Coordinate *c);
// update terrain with the damage if explosion happens at c
void damage(Terrain *terrain, Coordinate *c);
// returns the next true y
int16_t ceiling(Terrain *terrain, uint16_t x, uint16_t oldY);
// return the next false y
uint16_t closestGround(Terrain *terrain, uint16_t newX, uint16_t oldY);
// converts coordinate to index
uint32_t getIndex(uint16_t x, uint16_t y);
// converts index to coordinate
Coordinate getCoord(uint16_t idx);

#endif

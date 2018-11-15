#ifndef TERRAIN_H
#define TERRAIN_H
#include <stdbool.h>
#include "uart.h"

#define TERRAIN_WIDTH 320   // Screen width in pixels
#define TERRAIN_HEIGHT 240  // Screen width in pixels
#define TERRAIN_LENGTH 76800	// length of terrain array

typedef bool Terrain[TERRAIN_LENGTH];

void generateTerrain(Terrain *terrain);
bool collide(Terrain terrain, uint16_t x, uint16_t y);
void damage(Terrain *terrain, uint16_t x, uint16_t y);
// return the closest y value below given y
uint16_t closestY(Terrain terrain, uint16_t x, uint16_t y);

#endif

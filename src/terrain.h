#ifndef TERRAIN
#define TERRAIN
#include <stdbool.h>
#include "uart.h"
#include "coordinate.h"

#define TERRAIN_WIDTH 320   // Screen width in pixels
#define TERRAIN_HEIGHT 240  // Screen width in pixels
#define TERRAIN_LENGTH 76800	// length of terrain array

typedef bool Terrain[TERRAIN_LENGTH];

void generateTerrain(Terrain *terrain);
bool collide(Terrain terrain, Coordinate c);
void damage(Terrain *terrain, Coordinate c);
// return the closest y value below given y
uint16_t closestY(Terrain terrain, Coordinate c);

#endif

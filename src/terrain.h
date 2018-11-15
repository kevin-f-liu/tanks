#ifndef TERRAIN
#define TERRAIN
#include <stdbool.h>
#include "coordinate.h"
#include "uart.h"

#define TERRAIN_WIDTH 320                                // Screen width in pixels
#define TERRAIN_HEIGHT 240                               // Screen width in pixels
#define TERRAIN_LENGTH (TERRAIN_WIDTH * TERRAIN_HEIGHT)  // length of terrain array

typedef struct { char x[TERRAIN_LENGTH]; } Terrain;

void generateTerrain(Terrain *terrain);
bool collide(Terrain *terrain, Coordinate c);
void damage(Terrain *terrain, Coordinate c);
// return the closest y value below given y
uint16_t closestGround(Terrain *terrain, uint16_t newX, uint16_t oldY);

#endif

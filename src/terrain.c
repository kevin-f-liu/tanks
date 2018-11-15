#include "terrain.h"

uint32_t getIndex(uint16_t x, uint16_t y) { return TERRAIN_WIDTH * y + x; }

void generateTerrain(Terrain* terrain) {}
bool collide(Terrain* terrain, Coordinate c) {
  return terrain[getIndex(c.x, c.y)];
}
void damage(Terrain* terrain, Coordinate c) {
  // update terrain
}
uint16_t closestGround(Terrain *terrain, uint16_t x, uint16_t oldY){	
  uint32_t index = getIndex(x, oldY);
  uint16_t count = oldY;
  // while ground is not there
  while (index < TERRAIN_LENGTH) {
    if (terrain[index]) return count;
    count++;
    index += TERRAIN_WIDTH;
  }
  return TERRAIN_HEIGHT;
}

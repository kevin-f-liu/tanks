#include "terrain.h"

uint32_t getIndex(Coordinate c) { return TERRAIN_WIDTH * c.y + c.x; }

void generateTerrain(Terrain* terrain) {}
bool collide(Terrain terrain, Coordinate c) {
  return terrain[getIndex(c)];
}
void damage(Terrain* terrain, Coordinate c) {
  // update terrain
}
uint16_t closestY(Terrain terrain, Coordinate c) {
  uint32_t index = getIndex(c);
  uint8_t count = c.y;
  // while ground is not there
  while (index < TERRAIN_LENGTH) {
    if (terrain[index]) return count;
    count++;
    index += TERRAIN_WIDTH;
  }
  return TERRAIN_HEIGHT;
}

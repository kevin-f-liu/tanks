#include "terrain.h"

uint32_t getIndex(uint16_t x, uint16_t y) { return TERRAIN_WIDTH * y + x; }

void generateTerrain(Terrain* t) {}
bool collide(Terrain t, uint16_t x, uint16_t y) {
  return t[getIndex(x, y)];
}
void damage(Terrain* t, uint16_t x, uint16_t y) {
  // update terrain
}
uint16_t closestY(Terrain t, uint16_t x, uint16_t y) {
  uint32_t index = getIndex(x, y);
  uint8_t count = y;
  // while terrain is not there
  while (index < TERRAIN_LENGTH) {
    if (t[index]) return count;
    count++;
    index += TERRAIN_WIDTH;
  }
  return TERRAIN_HEIGHT;
}

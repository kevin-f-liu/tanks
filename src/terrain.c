#include "terrain.h"
#include <stdio.h>
#include "helper.h"
#include "player.h"

uint32_t getIndex(uint16_t x, uint16_t y) { return TERRAIN_WIDTH * y + x; }
Coordinate getCoord(uint16_t idx) {
	Coordinate c = {
		.x = idx % TERRAIN_WIDTH,
		.y = idx / TERRAIN_WIDTH
	};
	
	return c;
}

void generateTerrain(Terrain* terrain) {
  for (int i = 0; i < TERRAIN_LENGTH; i++) {
    terrain->x[i] = false;
  }
  uint16_t row = 2 * TERRAIN_HEIGHT / 3;
  for (int i = 0; i < TERRAIN_WIDTH; i++) {
    // randomly generated terrain, no idea if this works
    row += random(-1, 1);
    // prevent row 0 to be populated
    if (row == 0)
      row = 1;
    else if (row > TERRAIN_HEIGHT)
      row = TERRAIN_HEIGHT;
    for (int j = row; j < TERRAIN_HEIGHT; j++) {
      terrain->x[getIndex(i, j)] = true;
    }
  }
}
bool collide(Terrain* terrain, Coordinate* c) {
  return terrain->x[getIndex(c->x, c->y)];
}

void setFalse(Terrain* terrain, uint16_t x, uint16_t y) {
  if (x >= 0 && x < TERRAIN_WIDTH && y >= 0 && y < TERRAIN_HEIGHT) {
    terrain->x[getIndex(x, y)] = false;
  }
}

void damage(Terrain* terrain, Coordinate* c) {
  // update terrain
  uint16_t count = 0;
  for (int16_t i = RADIUS_OF_DAMAGE; i >= 0; i--) {
    setFalse(terrain, c->x + i, c->y);
    setFalse(terrain, c->x - i, c->y);
    for (uint16_t j = 1; j <= count; j++) {
      setFalse(terrain, c->x + i, c->y + j);
      setFalse(terrain, c->x + i, c->y - j);
      setFalse(terrain, c->x - i, c->y + j);
      setFalse(terrain, c->x - i, c->y - j);
    }
    count++;
  }
}
uint16_t closestGround(Terrain* terrain, uint16_t x, uint16_t oldY) {
  uint32_t index = getIndex(x, oldY);
  uint16_t count = oldY;
  // hit wall, go up
  if (terrain->x[index]) {
    while (index > 0) {
      if (!terrain->x[index]) return count;
      count--;
      index -= TERRAIN_WIDTH;
    }
    return 0;
  }

  // middle of air, go down
  while (index < TERRAIN_LENGTH) {
    if (terrain->x[index]) return count - 1;
    count++;
    index += TERRAIN_WIDTH;
  }
  return TERRAIN_HEIGHT;
}

void printTerrain(Terrain* terrain) {
  for (int i = 0; i < TERRAIN_LENGTH; i++) {
    if (i % TERRAIN_WIDTH == 0) printf("\n");
    printf("%d ", terrain->x[i]);
  }
  printf("\n");
}
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
	int8_t prevRand = 0, curRand = 0;
  for (int i = 0; i < TERRAIN_WIDTH; i++) {
		curRand = random(-1, 1);
		if (curRand && (curRand == -prevRand)) {
			curRand = 0;
		}
    row += curRand;
		prevRand = curRand;
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

void setFalse(Terrain* terrain, int32_t x, int32_t y) {
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

//return -1 if no ceiling
int16_t ceiling(Terrain* terrain, uint16_t x, uint16_t oldY){
	int32_t index = getIndex(x, oldY);
  uint16_t count = oldY;
  // middle of air, go up
	if (!terrain->x[index]) {
    while (index >= 0) {
      if (terrain->x[index]) return count;
      count--;
      index -= TERRAIN_WIDTH;
    }
    return -1;
  }
	// hit wall
  while (index < TERRAIN_LENGTH) {
    if (!terrain->x[index]) return count;
    count++;
    index += TERRAIN_WIDTH;
  }
  return TERRAIN_HEIGHT;
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

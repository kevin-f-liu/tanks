#include "graphics.h"
#include <cmsis_os.h>
#include <lpc17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GLCD.h"
#include "coordinate.h"
#include "helper.h"
#include "player.h"
#include "sprites.h"
#include "terrain.h"

typedef struct {
  int px;
  int py;
  char width;
  char height;
  const uint16_t *spritemap;  // Location of spritemap, which is constant
} base_sprite;

typedef struct {
  base_sprite base;
  int angle;
  int barrelOffset;
  Coordinate *pc;
} tank_graph_t;

typedef struct {
  base_sprite base;
  int angle;
} barrel_graph_t;

typedef struct {
  base_sprite base;
} shot_graph_t;

typedef struct {
  int8_t val;  // out of 100
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint16_t color;
} value_bar_t;

// Globals
tank_graph_t *t1, *t2;
barrel_graph_t *b1, *b2;
shot_graph_t *shot;
value_bar_t *p1_hp, *p2_hp, *power;

Terrain *t;

void initGraphics(uint16_t cColor, uint16_t bColor, uint16_t tColor, Terrain *te) {
	t = te;
  // Should also take in Allison's objects to init
  // Init LCD module for use, with a background color and textcolor
  GLCD_Init();
  GLCD_Clear(cColor);
  GLCD_SetTextColor(tColor);
  GLCD_SetBackColor(bColor);

  // Init all sprites and locations
  t1 = malloc(sizeof(tank_graph_t));
  t2 = malloc(sizeof(tank_graph_t));
  t1->pc = malloc(sizeof(Coordinate));
  t2->pc = malloc(sizeof(Coordinate));
  b1 = malloc(sizeof(barrel_graph_t));
  b2 = malloc(sizeof(barrel_graph_t));
  shot = malloc(sizeof(shot_graph_t));
  p1_hp = malloc(sizeof(value_bar_t));
  p2_hp = malloc(sizeof(value_bar_t));
  power = malloc(sizeof(value_bar_t));

  initBarrelmap(1);

  // Set all spritemaps
  t1->base.spritemap = tankmap;
  t2->base.spritemap = tankmap;
  b1->base.spritemap = barrelmap;
  b2->base.spritemap = barrelmap;
  shot->base.spritemap = shotmap;
  // Init tanks
  t1->base.px = 0;
  t1->base.py = TANK_WIDTH - TANK_HEIGHT;
  t1->base.width = TANK_WIDTH;
  t1->base.height = TANK_HEIGHT;
  t1->barrelOffset = TANK_WIDTH - TANK_HEIGHT;
  t1->pc->x = 0;  // Kinda wrong, but overwritten very quickly
  t1->pc->y = 0;
  // Init barrels
  b1->base.px = 0;
  b1->base.py = 0;
  b1->base.width = TANK_WIDTH;
  b1->base.height = TANK_WIDTH;
  // Init shot
  shot->base.px = 0;
  shot->base.py = 0;
  shot->base.width = SHOT_WIDTH;
  shot->base.height = SHOT_WIDTH;
  // Init bars, it's all constants so this can be technically removed
  p1_hp->x = 0;
  p1_hp->y = 10;
  p2_hp->x = TERRAIN_WIDTH_PX - VALUE_BAR_WIDTH;
  p1_hp->y = 10;
  p2_hp->y = 10;
  power->x = TERRAIN_WIDTH_PX / 2 - VALUE_BAR_WIDTH / 2;  // Centered
  power->y = 10;
  p1_hp->height = VALUE_BAR_HEIGHT;
  p2_hp->height = VALUE_BAR_HEIGHT;
  power->height = VALUE_BAR_HEIGHT;
  p1_hp->width = VALUE_BAR_WIDTH;
  p2_hp->width = VALUE_BAR_WIDTH;
  power->width = VALUE_BAR_WIDTH;
  p1_hp->color = P1_HP_COLOR;
  p2_hp->color = P2_HP_COLOR;
  power->color = POWER_COLOR;
  p1_hp->val = MAX_BAR_VAL + 1;
  p2_hp->val = MAX_BAR_VAL + 1;
  power->val = MAX_BAR_VAL + 1;  // Just need a value that is higher than max
}

void displayStringToLCD(int row, int column, int sz, char *str, int clear) {
  // Display a string to a position on the LCD, clears the amount of characters
  // needed.
  if (clear) {
    char *blanks = (char *)malloc(clear + 1);  // Allocate string of size clear
    // Fill with spaces and null termination char
    for (int i = 0; i < clear; i++) blanks[i] = ' ';
    blanks[clear] = '\0';
    GLCD_DisplayString(row, column, sz,
                       blanks);  // clear existing text, exactly as many needed
    free(blanks);
    blanks = NULL;
  }

  GLCD_DisplayString(row, column, sz, str);
}

uint16_t pixelFromCoord(uint16_t val) { return val * PX_PER_BLOCK; }

void displayBitmapToLCD(int col, int row, int width, int height,
                        const uint16_t *bitmap) {
  GLCD_Bitmap(col, row, width, height,
              (unsigned char *)bitmap);  // Allow larger bitmaps in
}

void displayBlock(int x, int y, const uint16_t *bitmap) {
  // Give block as terrain coordinate
  displayBitmapToLCD(pixelFromCoord(x), pixelFromCoord(y), PX_PER_BLOCK,
                     PX_PER_BLOCK, bitmap);
}

void drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
              uint16_t color) {
  if (!width || !height) return;
  // draw a rectangle of pixels really fast
  int i, j;

  GLCD_SetWindow(x, y, width, height);

  wr_cmd(0x22);
  wr_dat_start();
  for (i = (height - 1) * width; i > -1; i -= width) {
    for (j = 0; j < width; j++) {
      wr_dat_only(color);
    }
  }
  wr_dat_stop();
}

void clearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  drawRect(x, y, width, height, BACKGROUND_COLOR);
}

void clearBlock(int x, int y) {
  clearRect(pixelFromCoord(x), pixelFromCoord(y), PX_PER_BLOCK, PX_PER_BLOCK);
}

const uint16_t *getTerrainMap(Terrain *t, Coordinate c) {
  // Get the classification of a terrain coordinate, as in peak/slope etc and
  // return a pointer to its map
  if (isEdge(c, TERRAIN_WIDTH, TERRAIN_HEIGHT)) return terrainFull;
  if (t->x[getIndex(c.x, c.y + 1)] && !t->x[getIndex(c.x, c.y - 1)]) {
    // Bottom filled
    if (t->x[getIndex(c.x + 1, c.y)] && !t->x[getIndex(c.x - 1, c.y)]) {
      return terrainRSlope;
    }
    if (t->x[getIndex(c.x - 1, c.y)] && !t->x[getIndex(c.x + 1, c.y)]) {
      return terrainLSlope;
    }
  }
  return terrainFull;
}

void drawTerrainSection(Terrain *t, Coordinate c, uint16_t width,
                        uint16_t height) {
  // Render terrain for a specific section
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      Coordinate temp = {.x = c.x + j, .y = c.y + i};
      if (t->x[getIndex(temp.x, temp.y)]) {
        displayBlock(temp.x, temp.y, getTerrainMap(t, temp));
      }
    }
  }
}

void drawTerrain(Terrain *t) {
  Coordinate o = {.x = 0, .y = 0};
  drawTerrainSection(t, o, TERRAIN_WIDTH, TERRAIN_HEIGHT);
}

void updateTank(int newX, int newY, int newAng, char player) {
  // x and y are the top left corner of a widthXwidth square. Same as barrel
  tank_graph_t *tank = player == 1 ? t1 : t2;
  barrel_graph_t *barrel = player == 1 ? b1 : b2;

  // Update barrel first, if invalid angle, then don't change it
  if (newAng <= 180 && newAng >= -180) {
    loadBarrelmap(newAng);
  }

  // Barrel map completely overlaps tank bitmap, so clear that one
  clearRect(barrel->base.px, barrel->base.py, barrel->base.width,
            barrel->base.height);

  // Barrel update should go first to draw tank on top of barrel
  displayBitmapToLCD(newX, newY, barrel->base.width, barrel->base.height,
                     barrel->base.spritemap);
  displayBitmapToLCD(newX, newY + tank->barrelOffset, tank->base.width,
                     tank->base.height, tank->base.spritemap);

  barrel->base.px = newX;
  barrel->base.py = newY;
  tank->base.px = newX;
  tank->base.py = newY + tank->barrelOffset;
}

void moveTank(Coordinate c, char player) {
  // Animate tank between terrain coordinates
  tank_graph_t *tank = player == 1 ? t1 : t2;
  barrel_graph_t *barrel = player == 1 ? b1 : b2;

  // Rereference tank so bottom is on the coordinate
  int xPx = pixelFromCoord(c.x) - (TANK_WIDTH / 2 - PX_PER_BLOCK / 2);
  int yPx = pixelFromCoord(c.y) - (TANK_WIDTH - PX_PER_BLOCK);

  updateTank(xPx, yPx, 1000, player);

  Coordinate temp = {.x = 0, .y = 0};
  updateCoordinate(&temp, min(tank->pc->x, c.x) - 4, min(tank->pc->y, c.y) - 7);
  int width = max(tank->pc->x, c.x) - min(tank->pc->x, c.x) + 9;
  int height = max(tank->pc->y, c.y) - min(tank->pc->y, c.y) + 8;
  drawTerrainSection(t, temp, width, height);

  // Set tank coord
  tank->pc->x = c.x;
  tank->pc->y = c.y;
}

void aimTank(int newAng, char player) {
  // Animate tank barrel rotation
  tank_graph_t *tank = player == 1 ? t1 : t2;
  barrel_graph_t *barrel = player == 1 ? b1 : b2;

  // use the coordinate of the barrel so that tank does not move
  updateTank(barrel->base.px, barrel->base.py, newAng, player);
}

void initTank(Coordinate c, int angle, int player) {
  moveTank(c, player);
  aimTank(angle, player);
}

void updateShot(int newX, int newY) {
  // Update to pixel coordinate
  clearRect(shot->base.px, shot->base.py, shot->base.width, shot->base.height);
  displayBitmapToLCD(newX, newY, shot->base.width, shot->base.height,
                     shot->base.spritemap);
  shot->base.px = newX;
  shot->base.py = newY;
}

void explodeOrClear(int x, int y, char run, const uint16_t *map) {
  run % 2 ? clearBlock(x, y) : displayBlock(x, y, map);
}

void animateExplosion(Coordinate c, Terrain *t) {
  // Actual animation of the explosion
  // Need to be signaled by the game logic in order update the terrain map and
  // remove specifically the terrain update terrain
  const uint16_t *map = explode1map;
  for (char run = 0; run < 2; run++) {
    uint16_t count = RADIUS_OF_DAMAGE;
    for (int16_t i = 0; i <= RADIUS_OF_DAMAGE; i++) {
      map = i % 2 ? explode1map : explode2map;
      explodeOrClear(c.x + i, c.y, run, map);
      explodeOrClear(c.x - i, c.y, run, map);
      for (uint16_t j = 1; j <= count; j++) {
        map = i % 2 ? explode2map : explode1map;
        explodeOrClear(c.x + i, c.y + j, run, map);
        explodeOrClear(c.x + i, c.y - j, run, map);
        explodeOrClear(c.x - i, c.y + j, run, map);
        explodeOrClear(c.x - i, c.y - j, run, map);
        busyWait(100000);
      }
      count--;
    }
  }
}

void impact(Coordinate c, Terrain *t) {
  // Animate explosion impact and rerender terrain
  // Should render both tanks as well maybe?
  animateExplosion(c, t);
  updateCoordinate(&c, c.x - RADIUS_OF_DAMAGE, c.y - RADIUS_OF_DAMAGE);
  drawTerrainSection(t, c, 2 * RADIUS_OF_DAMAGE + 1, 2 * RADIUS_OF_DAMAGE + 1);
}

uint16_t barValueToPixels(uint8_t val) {
  return (int)((double)val * (double)VALUE_BAR_WIDTH / (double)MAX_BAR_VAL);
}

void updateBar(value_bar_t *b, uint8_t newVal) {
  if (b->val > MAX_BAR_VAL) {
    // Init
    // printf("INIT: %d %d %d %d\n", b->x + barValueToPixels(newVal), b->y,
    // barValueToPixels(MAX_BAR_VAL - newVal), b->height);
    drawRect(b->x, b->y, barValueToPixels(newVal), b->height, b->color);
    drawRect(b->x + barValueToPixels(newVal), b->y,
             barValueToPixels(MAX_BAR_VAL - newVal), b->height,
             EMPTY_BAR_COLOR);
    b->val = newVal;
    return;
  }

  uint16_t dif = abs(newVal - b->val);
  uint16_t difpx = barValueToPixels(dif);
  uint16_t prevVX = b->x + barValueToPixels(b->val);

  if (!difpx) {
    return;
  }

  if (newVal < b->val) {
    drawRect(prevVX - difpx - 1, b->y, difpx + 1, b->height, EMPTY_BAR_COLOR);
  } else if (newVal > b->val) {
    drawRect(prevVX - 1, b->y, difpx + 1, b->height, b->color);
  }

  b->val = newVal;
}

void updateHealthBar(uint8_t newVal, int player) {
  value_bar_t *bar = player == 1 ? p1_hp : p2_hp;
  updateBar(bar, newVal);
}

void updatePowerBar(uint8_t newVal) { updateBar(power, newVal); }

void drawPermText(void) {
  displayStringToLCD(0, 3, 0, "Player 1", 0);
  displayStringToLCD(0, 24, 0, "Power", 0);
  displayStringToLCD(0, 42, 0, "Player 2", 0);
}

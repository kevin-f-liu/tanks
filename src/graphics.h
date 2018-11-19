/*
 * graphics.h
 * Graphics helper functions header
 */

#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <stdbool.h>
#include "coordinate.h"
#include "terrain.h"
#include "uart.h"

// Define colors used
#define BACKGROUND_COLOR 0xFFFF
#define TERRAIN_COLOR 0x0000
#define TEXT_COLOR 0x0000

// Define terrain constants
#define TERRAIN_WIDTH_PX 320
#define TERRAIN_HEIGHT_PX 240
#define PX_PER_BLOCK 4

// Define bar params
#define MAX_BAR_VAL 100
#define VALUE_BAR_WIDTH 80
#define VALUE_BAR_HEIGHT 8
#define P1_HP_COLOR 0x2BDA
#define P2_HP_COLOR 0xBA08
#define POWER_COLOR 0xBA12
#define EMPTY_BAR_COLOR 0x6B4D

extern void initGraphics(uint16_t cColor, uint16_t bColor, uint16_t tColor);
void displayStringToLCD(int row, int column, int sz, char* str, int clear);
void drawTerrain(Terrain* t);
void initTank(Coordinate c, int angle, int player);
void updatePowerBar(uint8_t newVal);
void updateHealthBar(uint8_t newVal, int player);
void drawPermText(void);
void moveTank(Coordinate c, char player);
void aimTank(int newAng, char player);
void updateShot(Coordinate c);
void impact(Coordinate c, Terrain* t);

#endif /* _GRAPHICS_H */

/*
 * graphics.h
 * Graphics helper functions header
 */

#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <stdbool.h>
#include "uart.h"

<<<<<<< HEAD
#define TERRAIN_WIDTH 320 // Screen width in pixels
#define TERRAIN_HEIGHT 240 // Screen width in pixels

// Global terrain array
extern unsigned char terrain[TERRAIN_HEIGHT*TERRAIN_WIDTH];


extern void graphicsWorker(void const *arg);
=======
extern void graphicsWorker(void const* arg);
>>>>>>> a086b73729ac24a8c95cd980ce13e69edaa522cd
extern void initGraphics(uint16_t cColor, uint16_t bColor, uint16_t tColor);
void displayStringToLCD(int row, int column, int sz, char* str, int clear);

#endif /* _GRAPHICS_H */

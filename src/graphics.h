/*
 * graphics.h
 * Graphics helper functions header
 */

#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <stdbool.h>
#include "uart.h"

extern void graphicsWorker(void const* arg);
extern void initGraphics(uint16_t cColor, uint16_t bColor, uint16_t tColor);
void displayStringToLCD(int row, int column, int sz, char* str, int clear);

#endif /* _GRAPHICS_H */

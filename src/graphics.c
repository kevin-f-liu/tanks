#include <cmsis_os.h>
#include <lpc17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GLCD.h"
#include "graphics.h"
#include "sprites.h"

typedef struct {
    int px;
    int py;
    char width;
    char height;
    uint16_t *spritemap; // Location of spritemap, which is constant
} base_sprite;

typedef struct {
    base_sprite base;
    int angle;
} tank_graph_t;

typedef struct {
    base_sprite base;
    int angle;
} barrel_graph_t;

typedef struct {
    base_sprite base;
} shot_graph_t;

// Globals
unsigned char terrain[TERRAIN_HEIGHT*TERRAIN_WIDTH];
tank_graph_t *t1, *t2;
barrel_graph_t *b1, *b2;
shot_graph_t *shot;

unsigned char getTerrainVal(int x, int y) {
    return terrain[TERRAIN_WIDTH * y + x];
}

void initGraphics(uint16_t cColor, uint16_t bColor, uint16_t tColor) {
    // Should also take in Allison's objects to init
	// Init LCD module for use, with a background color and textcolor
	GLCD_Init();
	GLCD_Clear(cColor);
	GLCD_SetTextColor(tColor);
	GLCD_SetBackColor(bColor);
    
    // Init all sprites and locations
    t1 = malloc(sizeof(tank_graph_t));
    t2 = malloc(sizeof(tank_graph_t));
    b1 = malloc(sizeof(barrel_graph_t));
    b2 = malloc(sizeof(barrel_graph_t));
    shot = malloc(sizeof(shot_graph_t));
    
    t1->base.spritemap = tankmap;
    t2->base.spritemap = tankmap;
    
    t1->base.px = 0;
    t1->base.py = 0;
}

void clearRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    // Clear a rectangle of pixels really fast
}

void updateTank(int newX, int newY, char player) {
    tank_graph_t *tank = player == 1 ? t1 : t2;
    
    
}

void updateBarrel(int newX, int newY, int newAng, char player) {
    
}

void updateShot(int newX, int newY) {
}

void displayStringToLCD(int row, int column, int sz, char* str, int clear) {
	// Display a string to a position on the LCD, clears the amount of characters needed.
	if (clear) {
		char* blanks = (char*) malloc(clear + 1); // Allocate string of size clear
		// Fill with spaces and null termination char
		for (int i = 0; i < clear; i++) blanks[i] = ' ';
		blanks[clear] = '\0';
		GLCD_DisplayString(row, column, sz, blanks); // clear existing text, exactly as many needed
		free(blanks);
		blanks = NULL;
	}
	
	GLCD_DisplayString(row, column, sz, str);
}

void displayBitmapToLCD(int col, int row, int width, int height, uint16_t* bitmap) {
    GLCD_Bitmap(col, row, width, height, (unsigned char*) bitmap);
}



void graphicsWorker(void const *arg) {
	initGraphics(White, White, Black);
	int count = 0;
	char result[12]; // Temp storage string
    
    
	while(true) {
		sprintf(result, "%d", count);
		displayStringToLCD(5, 5, 0, result, 12);
        displayBitmapToLCD(count, count, 32, 13, tank_map);
		count++;
		osDelay(900);
        GLCD_Clear(White);
	}
}


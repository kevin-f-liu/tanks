#include <cmsis_os.h>
#include <lpc17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GLCD.h"
#include "graphics.h"
#include "sprites.h"
#include "terrain.h"

typedef struct {
    int px;
    int py;
    char width;
    char height;
    const uint16_t *spritemap; // Location of spritemap, which is constant
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
tank_graph_t *t1, *t2;
barrel_graph_t *b1, *b2;
shot_graph_t *shot;

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
	  b1->base.spritemap = barrelmap;
    b2->base.spritemap = barrelmap;
    
    t1->base.px = 0;
    t1->base.py = 0;
		t1->base.width = TANK_WIDTH;
	  t1->base.height = TANK_HEIGHT;
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

void displayBitmapToLCD(int col, int row, int width, int height, const uint16_t* bitmap) {
    GLCD_Bitmap(col, row, width, height, (unsigned char*) bitmap);
}

void clearRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    // Clear a rectangle of pixels really fast
	  int i, j;
		
		GLCD_SetWindow(x, y, width, height);

		wr_cmd(0x22);
		wr_dat_start();
		for (i = (height-1)*width; i > -1; i -= width) {
			for (j = 0; j < width; j++) {
				wr_dat_only(BACKGROUND_COLOR);
			}
		}
		wr_dat_stop();
}

void updateTank(int newX, int newY, char player) {
    tank_graph_t *tank = player == 1 ? t1 : t2;
    clearRect(tank->base.px, tank->base.py, tank->base.width, tank->base.height);
    displayBitmapToLCD(newX, newY, tank->base.width, tank->base.height, tank->base.spritemap);
	  tank->base.px = newX;
	  tank->base.py = newY;
}

void updateBarrel(int newX, int newY, int newAng, char player) {
    // Call the update barrel function, use global barrel map
}

void updateShot(int newX, int newY) {
}

void graphicsWorker(void const *arg) {
	initGraphics(BACKGROUND_COLOR, BACKGROUND_COLOR, Black);
	int count = 0;
	char result[12]; // Temp storage string
    
    
	while(true) {
		sprintf(result, "%d", count);
		displayStringToLCD(5, 5, 0, result, 12);
		
		updateTank(count, count, 1);
		count++;
		osDelay(3000);
	}
}


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
#include "coordinate.h"

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
	  int barrelOffset;
} tank_graph_t;

typedef struct {
    base_sprite base;
    int angle;
} barrel_graph_t;

typedef struct {
    base_sprite base;
} shot_graph_t;

typedef struct {
	int8_t val; // out of 100
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
		power->x = TERRAIN_WIDTH_PX / 2 - VALUE_BAR_WIDTH / 2; // Centered 
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
		p1_hp->val = MAX_BAR_VAL+1;
		p2_hp->val = MAX_BAR_VAL+1;
		power->val = MAX_BAR_VAL+1; // Just need a value that is higher than max
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

void drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
		if (!width || !height) return;
	  // draw a rectangle of pixels really fast
	  int i, j;
		
		GLCD_SetWindow(x, y, width, height);

		wr_cmd(0x22);
		wr_dat_start();
		for (i = (height-1)*width; i > -1; i -= width) {
			for (j = 0; j < width; j++) {
				wr_dat_only(color);
			}
		}
		wr_dat_stop();
}

void clearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    drawRect(x, y, width, height, BACKGROUND_COLOR);
}

void updateTank(int newX, int newY, int newAng, char player) {
    // x and y are the top left corner of a widthXwidth square. Same as barrel
    tank_graph_t *tank = player == 1 ? t1 : t2;
	  barrel_graph_t *barrel = player == 1 ? b1 : b2;
		
	  // Update barrel first
	  if (newAng <= 180 && newAng >= -180) {
			  loadBarrelmap(newAng);			  
		}
		
	  clearRect(barrel->base.px, barrel->base.py, barrel->base.width, barrel->base.height);
		
		// Barrel update should go first to draw tank on top of barrel
		displayBitmapToLCD(newX, newY, barrel->base.width, barrel->base.height, barrel->base.spritemap);
    displayBitmapToLCD(newX, newY + tank->barrelOffset, tank->base.width, tank->base.height, tank->base.spritemap);
		barrel->base.px = newX;
	  barrel->base.py = newY;
	  tank->base.px = newX;
	  tank->base.py = newY + tank->barrelOffset;
}

void updateShot(int newX, int newY) {
	  clearRect(shot->base.px, shot->base.py, shot->base.width, shot->base.height);
	  displayBitmapToLCD(newX, newY, shot->base.width, shot->base.height, shot->base.spritemap);
	  shot->base.px = newX;
	  shot->base.py = newY;
}

uint8_t classifyTerrain(Terrain *t, Coordinate c) {
	// Get the classification of a terrain coordinate, as in peak/slope etc
	
}

void drawTerrain(Terrain *t) {
	int pxRow = -1;
	for (int i = 0; i < TERRAIN_LENGTH; i++) {
    if (!(i % TERRAIN_WIDTH)) {
			// New line 
			pxRow++;
		}
		if (t->x[i]) {
			Coordinate c = getCoord(i);
			// TODO: Determine if it is a slope or not
			displayBitmapToLCD(c.x*PX_PER_BLOCK, c.y*PX_PER_BLOCK, PX_PER_BLOCK, PX_PER_BLOCK, terrainFull);
		}
	}
}

void destroyTerrain(Coordinate c, Terrain *t) {
	// Need to be signaled by the game logic in order update the terrain map and remove specifically the terrain
}

uint16_t barValueToPixels(uint8_t val) {
	return (int)((double)val * (double) VALUE_BAR_WIDTH / (double) MAX_BAR_VAL);
}

void updateBar(value_bar_t *b, uint8_t newVal) {
	if (b->val > MAX_BAR_VAL) {
		// Init
		//printf("INIT: %d %d %d %d\n", b->x + barValueToPixels(newVal), b->y, barValueToPixels(MAX_BAR_VAL - newVal), b->height); 
		drawRect(b->x, b->y, barValueToPixels(newVal), b->height, b->color);
		drawRect(b->x + barValueToPixels(newVal), b->y, barValueToPixels(MAX_BAR_VAL - newVal), b->height, EMPTY_BAR_COLOR); 
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
		drawRect(prevVX - difpx-1, b->y, difpx+1, b->height, EMPTY_BAR_COLOR);
	} else if (newVal > b->val) {
		drawRect(prevVX-1, b->y, difpx+1, b->height, b->color); 
	}
	
	b->val = newVal;
}

void updateHealthBar(uint8_t newVal, int player) {
	value_bar_t *bar = player == 1 ? p1_hp : p2_hp;
	updateBar(bar, newVal);
}

void updatePowerBar(uint8_t newVal) {
	updateBar(power, newVal);
}

void drawPermText(void) {
	displayStringToLCD(0, 3, 0, "Player 1", 0);
	displayStringToLCD(0, 24, 0, "Power", 0);
	displayStringToLCD(0, 42, 0, "Player 2", 0);
}

void graphicsWorker(void const *arg) {
	printf("starting graphics worker\n");
	Terrain *t = (Terrain *)arg;
	initGraphics(BACKGROUND_COLOR, BACKGROUND_COLOR, Black);
	int count = 0;
	char result[12]; // Temp storage string
  updateTank(90, 100, 60, 1);
	// Init power
	updatePowerBar(100);
	updateHealthBar(100,1);
	updateHealthBar(100,2);
	drawPermText();
	while(true) {
		sprintf(result, "%d", count);
		displayStringToLCD(5, 5, 0, result, 12);
		//updateTank(100, 100, 20*count % 180, 1);
		//updateShot(count, count);
		drawTerrain(t);
		updatePowerBar(100 - 10 * (count % 10));
		updateHealthBar(100 - count%100, 1);
		updateHealthBar( 10* (count%10), 2);
		count++;
		osDelay(300);
	}
}


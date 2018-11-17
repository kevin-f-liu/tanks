#include <cmsis_os.h>
#include <lpc17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "GLCD.h"
#include "graphics.h"
#include "sprites.h"
#include "math.h"
#include "helper.h"

 const uint16_t tankmap[] = {
  0xffff, 0xffde, 0xffff, 0xf79e, 0xffff, 0xffdf, 0xffff, 0x0000, 0x0000, 0x0041, 0x0000, 0x0021, 0x0021, 0x0000, 0x0020, 0x0000, 
	0x0000, 0x0861, 0x0000, 0x0000, 0x0021, 0x0000, 0x0001, 0x0000, 0x0001, 0x0020, 0xf79d, 0xffff, 0xffff, 0xffff, 0xffff, 0xf79e, 
	0xffff, 0xf7be, 0xffff, 0xffff, 0xef3c, 0x0820, 0x18c3, 0x4a28, 0x4a49, 0x4208, 0x0800, 0x0841, 0x41e7, 0x1062, 0x0000, 0x4a48, 
	0x4a28, 0x4207, 0x0820, 0x1061, 0x41e7, 0x1062, 0x0000, 0x4a49, 0x41c7, 0x0800, 0x20c2, 0xd679, 0xffff, 0xf79e, 0xe71c, 0xffff, 
	0xdf3d, 0xffff, 0xd69b, 0x20c3, 0x2082, 0x0800, 0x4186, 0x5a49, 0x5208, 0x1000, 0x1000, 0x4165, 0x628a, 0x49e7, 0x1841, 0x0800, 
	0x5208, 0x1000, 0x1000, 0x4186, 0x62aa, 0x49e7, 0x1041, 0x0800, 0x5a48, 0x5a28, 0x1000, 0x5a08, 0xc575, 0xffff, 0xffff, 0xef7e, 
	0xffff, 0xce59, 0x49e8, 0x1000, 0x1800, 0x3041, 0x3861, 0x2800, 0x3000, 0x2800, 0x3800, 0x3800, 0x2800, 0x2800, 0x2800, 0x2800, 
	0x3000, 0x2000, 0x3000, 0x3000, 0x2000, 0x2800, 0x2800, 0x2800, 0x2800, 0x4061, 0x3800, 0x4882, 0x61a7, 0xe639, 0xef1c, 0xffff, 
	0xf6fb, 0x30a2, 0x1800, 0x71c7, 0x926a, 0x7104, 0x70c3, 0x8986, 0x8944, 0x9165, 0x9104, 0x9125, 0xa166, 0x9145, 0x8925, 0x8925, 
	0x8945, 0x9166, 0x8925, 0x9125, 0x9966, 0x9145, 0x9145, 0x8944, 0x9145, 0x78a2, 0xa1c7, 0x9186, 0x6904, 0x3841, 0x30c2, 0xff5c, 
	0x2061, 0x1800, 0x5124, 0x7a08, 0x5882, 0x68c3, 0x89a6, 0x8144, 0x9165, 0x8924, 0x9104, 0xa145, 0x98c4, 0x98a3, 0xa945, 0x9904, 
	0x98e4, 0x98e3, 0x98e4, 0xa145, 0x98e4, 0x90e3, 0x9965, 0x9165, 0xa1a6, 0x8904, 0x8904, 0x8945, 0x89e8, 0x5945, 0x1000, 0x1020, 
	0x0000, 0x18a2, 0x0800, 0x30a3, 0x1800, 0x1800, 0x2800, 0x2800, 0x2800, 0x4000, 0x5000, 0x5000, 0x6800, 0x6800, 0x6800, 0x7000, 
	0x6800, 0x7000, 0x6800, 0x5800, 0x6020, 0x5040, 0x3000, 0x2800, 0x2000, 0x3861, 0x2800, 0x3841, 0x1800, 0x2862, 0x0000, 0x0000, 
	0xffff, 0xef9e, 0x1061, 0x0000, 0x1021, 0x0800, 0x1020, 0x0800, 0x1000, 0x2800, 0x6965, 0x8986, 0x9124, 0xa124, 0x98c3, 0xa103, 
	0xa104, 0x98c3, 0xa145, 0x8924, 0x8186, 0x6165, 0x2000, 0x1000, 0x1841, 0xf73c, 0xffdf, 0xff3c, 0x2082, 0x0000, 0x0020, 0xffff, 
	0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0xffbe, 0x0840, 0x1020, 0x1800, 0x4000, 0x9186, 0x98a3, 0xb0e4, 0xb882, 0xc0a3, 
	0xb882, 0xb8c4, 0xa8e4, 0x9945, 0x8125, 0x3000, 0x2020, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0x0820, 0xffff, 0xffff, 0xffff, 0xffff, 0xef7d, 0xef3c, 0x4985, 0x2000, 0x7924, 0xa9a7, 0xa904, 0xa041, 0xc0e4, 
	0xb0a3, 0xb105, 0x98c3, 0x8924, 0x3000, 0x38a2, 0xf71c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0xefdf, 0xf7ff, 0xffff, 0xc5f7, 0x4145, 0x2000, 0x3000, 0x7924, 0xa248, 0x8103, 
	0x8986, 0x8165, 0x3800, 0x5103, 0xff3c, 0xffff, 0xef3d, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xf7be, 0x2124, 0xef5d, 0xffff, 0xffff, 0xffff, 0xf7ff, 0xe77d, 0xffff, 0xffff, 0xde59, 0x5186, 0x1800, 0x2800, 0x3000, 
	0x2000, 0x3882, 0xf6ba, 0xffde, 0xf79d, 0xffff, 0xffff, 0xf7df, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xe73c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce38, 0x3104, 0x0800, 0x1000, 
	0x2061, 0xff5c, 0xffff, 0xffff, 0xffff, 0xf7ff, 0xef9e, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
};
 
uint16_t barrelmap[TANK_WIDTH*TANK_WIDTH];

void printBarrelmap(void) {
	for (int idx = 0; idx < TANK_WIDTH * TANK_WIDTH; idx++) {
		printf("%x ", barrelmap[idx]);
		if (!(idx % TANK_WIDTH)) printf("\n");
	}
}

void initBarrelmap(char player) {
	loadBarrelmap(player == 1 ? 0 : 180);
}

void loadBarrelmap(int a) {
	// Generate a sprite map that is tank_width^2 size with a line of pixels with the angle specified, from the middle
	
	double angle = toRad(a);
	int idx;
	
	int startx = TANK_WIDTH / 2;
	int starty = TANK_HEIGHT;
	
	int x, y, h2;
	double cursin, curcos;
  double sinang = sin(angle);
	double cosang = cos(angle);
	double tol = 0.1;
	
	for (idx = 0; idx < TANK_WIDTH * TANK_WIDTH; idx++) {
		x = idx % TANK_WIDTH - startx;
		y = idx / TANK_WIDTH - starty;
		h2 = x*x+y*y;
		cursin = y*fastInvsqrt(h2);
		curcos = x*fastInvsqrt(h2);
		//printf("x: %d y: %d h2: %f cursin: %f curcos: %f \n", x, y, h2, cursin, curcos);
		if (h2 <= BARREL_LENGTH2 && 
			  cursin <= sinang + tol && cursin >= sinang - tol &&
		    curcos <= cosang + tol && curcos >= cosang - tol) {
			barrelmap[idx] = BARREL_COLOR;
		} else {
			barrelmap[idx] = BACKGROUND_COLOR;
		}
	}
}

const uint16_t shotmap[] = {
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
};





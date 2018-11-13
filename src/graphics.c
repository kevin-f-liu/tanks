#include <cmsis_os.h>
#include <lpc17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "GLCD.h"
#include "graphics.h"

void initGraphics(uint16_t cColor, uint16_t bColor, uint16_t tColor) {
	// Init LCD module for use, with a background color and textcolor
	GLCD_Init();
	GLCD_Clear(cColor);
	GLCD_SetTextColor(tColor);
	GLCD_SetBackColor(bColor);
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

//void displayBitmapToLCD(int row, int col, 



void graphicsWorker(void const *arg) {
	initGraphics(Blue, Blue, White);
	int count = 0;
	char result[12]; // Temp storage string
	while(true) {
		sprintf(result, "%d", count);
		displayStringToLCD(5, 5, 0, result, 12);
		count++;
		osDelay(9000);
	}
}


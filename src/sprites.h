/*
* sprites.h
* Graphics helper functions header 
*/

#ifndef _SPRITE_H
#define _SPRITE_H

#define TANK_WIDTH 32
#define TANK_HEIGHT 13
extern const uint16_t tankmap[TANK_HEIGHT*TANK_WIDTH];
extern uint16_t barrelmap[TANK_WIDTH*TANK_WIDTH];
extern void load_barrelmap(int angle);
extern void init_barrelmap(void);
#endif /* _SPRITE_H */

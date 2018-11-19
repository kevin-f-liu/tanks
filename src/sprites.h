/*
* sprites.h
* Graphics helper functions header 
*/

#ifndef _SPRITE_H
#define _SPRITE_H

#define TANK_WIDTH 32
#define TANK_HEIGHT 13
#define BARREL_WIDTH TANK_WIDTH
#define BARREL_HEIGHT TANK_WIDTH
#define BARREL_LENGTH2 196
#define BARREL_COLOR 0x0000
#define SHOT_WIDTH 4
#define TERRAIN_BLOCK_WIDTH 4

extern const uint16_t tankmap1[TANK_HEIGHT*TANK_WIDTH];
extern const uint16_t tankmap2[TANK_HEIGHT*TANK_WIDTH];
extern uint16_t barrelmap[TANK_WIDTH*TANK_WIDTH];
extern const uint16_t shotmap[SHOT_WIDTH*SHOT_WIDTH];
extern const uint16_t terrainRSlope[TERRAIN_BLOCK_WIDTH*TERRAIN_BLOCK_WIDTH];
extern const uint16_t terrainLSlope[TERRAIN_BLOCK_WIDTH*TERRAIN_BLOCK_WIDTH];
extern const uint16_t terrainFull[TERRAIN_BLOCK_WIDTH*TERRAIN_BLOCK_WIDTH];
extern const uint16_t explode1map[TERRAIN_BLOCK_WIDTH*TERRAIN_BLOCK_WIDTH];
extern const uint16_t explode2map[TERRAIN_BLOCK_WIDTH*TERRAIN_BLOCK_WIDTH];
extern void loadBarrelmap(int angle);
extern void initBarrelmap(char player);
#endif /* _SPRITE_H */

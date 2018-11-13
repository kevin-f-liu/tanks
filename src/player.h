#include "uart.h"
#ifndef PLAYER
#define PLAYER

typedef struct {
    // HP max 100
    int8_t HP;
    uint16_t x;
    uint16_t y;
    //-179 to 180
    int16_t aimAngle;
} Player;

void updateHealth(Player *p, uint16_t landX, uint16_t landY);
void updatePosition(Player *p, uint32_t terrain);

#endif
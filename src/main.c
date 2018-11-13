#include <cmsis_os.h>
#include <lpc17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "GLCD.h"
#include "player.h"

Player p1, p2;

// a number between 1 to 100
uint8_t firepower = 100;
bool isP1 = true;

// change in horizontal position
uint8_t changePos = 0;
// change in aim
uint8_t changeAim = 0;
// current potentiometer
uint16_t potValue;
bool newGame = false;

void potentiometerWorker(void const *arg) {
    LPC_PINCON->PINSEL1 &= ~(0x03 << 18);
    LPC_PINCON->PINSEL1 |= (0x01 << 18);

    LPC_SC->PCONP |= (0x01 << 12);

    LPC_ADC->ADCR = (1 << 2) |  // select AD0.2 pin
                    (4 << 8) |  // ADC clock is 25MHz/5
                    (1 << 21);  // enable ADC
    while (1) {
        LPC_ADC->ADCR |= (0x01 << 24);
        while (~LPC_ADC->ADGDR & (0x01 << 31))
            ;
        potValue = (LPC_ADC->ADGDR & (0xfff << 4)) >> 4;
        printf("%d\n", potValue);
        osThreadYield();
    }
}

void joystickWorker(void const *arg) {
    while (1) {
        switch (~(LPC_GPIO1->FIOPIN) & 0x07800000) {
            case 0x01000000:
                changeAim++;
                break;
            case 0x00800000:
                changePos++;
                break;
            case 0x02000000:
                changePos--;
                break;
            case 0x04000000:
                changeAim--;
                break;
            default:
                break;
        }
        printf("%d, %d\n", changePos, changeAim);
        osThreadYield();
    }
}

// TODO: signal semaphore or reset game
void pushbuttonWorker(void const *arg) {
    bool state = false;
    bool lastButtonState = false;
    while (true) {
        if ((~LPC_GPIO2->FIOPIN & (0x01 << 10)) != lastButtonState &&
            !lastButtonState) {
            lastButtonState = true;
            if (newGame) {
                // reset game
            } else {
            }

        } else if (!(~LPC_GPIO2->FIOPIN & (0x01 << 10))) {
            lastButtonState = false;
        }
        osThreadYield();
    }
}

void gameWorker(void const *arg) {
    while (true) {
        // TODO: wait for semaphore
        // Use current values for projectile and stuff
        // check if game ends
        osThreadYield();
    }
}

osThreadDef(potentiometerWorker, osPriorityNormal, 1, 0);
osThreadDef(joystickWorker, osPriorityNormal, 1, 0);
osThreadDef(pushbuttonWorker, osPriorityNormal, 1, 0);
// maybe make this high
osThreadDef(gameWorker, osPriorityNormal, 1, 0);
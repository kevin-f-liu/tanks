#include <cmsis_os.h>
#include <lpc17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "GLCD.h"
#include "graphics.h"
#include "helper.h"
#include "player.h"

Player p1, p2;

// a number between 1 to 100
uint8_t firepower = 100;
bool isP1 = true;

// current potentiometer
uint16_t potValue;
bool newGame = true;

osSemaphoreId semaphore;
osSemaphoreDef(semaphore);

Terrain terrain;

void setupGame() {
  // TODO: generate terrain here
  p1.HP = 100;
  p1.aimAngle = 0;
  updatePosition(&p1, random(0, 160), terrain);

  p2.HP = 100;
  p2.aimAngle = 180;
  updatePosition(&p2, random(160, 319), terrain);
  printf("Pos: %d, %d\n", p1.x, p2.x);
  printf("Aim: %d, %d\n", p1.aimAngle, p2.aimAngle);
}

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
    firepower = ((double)potValue) / 4096 * 100 + 1;
    osThreadYield();
  }
}

void joystickWorker(void const *arg) {
  while (1) {
    // change in horizontal position
    int8_t changePos = 0;
    // change in aim
    int8_t changeAim = 0;
    bool delay = true;
    switch (~(LPC_GPIO1->FIOPIN) & 0x07800000) {
      case 0x01000000:
        changePos++;
        break;
        // down is counter clockwise (25)
      case 0x00800000:
        changeAim--;
        break;
        // up is clockwise (23)
      case 0x02000000:
        changeAim++;
        break;
      case 0x04000000:
        changePos--;
        break;
      default:
        delay = false;
        break;
    }
    if (delay) {
      if (isP1) {
        updatePosition(&p1, changePos + p1.x, terrain);
        updateAim(&p1, changeAim + p1.aimAngle);
      } else {
        updatePosition(&p2, changePos + p2.x, terrain);
        updateAim(&p2, changeAim + p2.aimAngle);
      }
      osDelay(1000);
    }
  }
}

void pushbuttonWorker(void const *arg) {
  bool state = false;
  bool lastButtonState = false;
  while (true) {
    if ((~LPC_GPIO2->FIOPIN & (0x01 << 10)) != lastButtonState &&
        !lastButtonState) {
      lastButtonState = true;
      if (newGame) {
        // reset game
        setupGame();
        newGame = false;
      } else {
        // signal semaphore
        osSemaphoreRelease(semaphore);
      }

    } else if (!(~LPC_GPIO2->FIOPIN & (0x01 << 10))) {
      lastButtonState = false;
    }
    osThreadYield();
  }
}

void gameWorker(void const *arg) {
  while (true) {
    // wait for semaphore
    osSemaphoreWait(semaphore, osWaitForever);
    printf("Pos: %d, %d\n", p1.x, p2.x);
    printf("Aim: %d, %d\n", p1.aimAngle, p2.aimAngle);
    printf("Firepower: %d\n", firepower);
    busyWait(10000000);
    // Use current values for projectile and stuff
    // check if game ends
    if (p1.HP <= 0 || p2.HP <= 0) {
      newGame = true;
    } else {
      // switch turn
      isP1 = !isP1;
    }
    osThreadYield();
  }
}

osThreadDef(potentiometerWorker, osPriorityNormal, 1, 0);
osThreadDef(joystickWorker, osPriorityNormal, 1, 0);
osThreadDef(pushbuttonWorker, osPriorityNormal, 1, 0);
// test: semaphore should block game worker
// once semaphore is available run the animation and calculations
osThreadDef(gameWorker, osPriorityHigh, 1, 0);
// graphics thread should have a high priority as well, but has a delay so it
// wont block the other stuff?
osThreadDef(graphicsWorker, osPriorityNormal, 1, 0);

int main(void) {
  // Unit UART printing,
  // Init kernel
  // Init 3 threads for data collection, processing, and sending
  printf("Starting\n");
  osKernelInitialize();
  osKernelStart();
  semaphore = osSemaphoreCreate(osSemaphore(semaphore), 0);
  osThreadId t1 = osThreadCreate(osThread(potentiometerWorker), NULL);
  osThreadId t2 = osThreadCreate(osThread(joystickWorker), NULL);
  osThreadId t3 = osThreadCreate(osThread(pushbuttonWorker), NULL);
  osThreadId t4 = osThreadCreate(osThread(gameWorker), NULL);
  osThreadId t5 = osThreadCreate(osThread(graphicsWorker), NULL);

  // Continue that main thread forever
  while (1)
    ;
}
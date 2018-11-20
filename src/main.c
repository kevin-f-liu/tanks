#include "GLCD.h"
#include "graphics.h"
#include "helper.h"
#include "player.h"
#include "terrain.h"
#include <cmsis_os.h>
#include <lpc17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

Player p1, p2;

// a number between 1 to 10
uint8_t firepower = MAX_FIREPOWER;
bool isP1 = true;
bool newGame = false;
// keep track of whether variables can be updated by peripherals
bool wait = false;
bool collided = false;

osSemaphoreId semaphore;
osSemaphoreDef(semaphore);

osSemaphoreId graphics;
osSemaphoreDef(graphics);

Terrain terrain;
// coordinate of the ball
// ball will update as player pos is updated (hide the ball)
Coordinate ball;

void setupGame() {
  // generate terrain here
  wait = true;
  generateTerrain(&terrain);
  setupPlayer(&p1, true);
  updatePosition(&p1, random(0, TERRAIN_WIDTH / 4), &terrain);
  // ball = p1.pos;
  hideShot(&ball);
  setupPlayer(&p2, false);
  updatePosition(&p2, random(3 * TERRAIN_WIDTH / 4, TERRAIN_WIDTH), &terrain);
  printf("Pos: (%d,%d), (%d,%d)\n", p1.pos.x, p1.pos.y, p2.pos.x, p2.pos.y);

  // graphics
  resetGraphics();
  drawTerrain(&terrain);
  // Init tanks
  initTank(p1.pos, p1.aimAngle, 1);
  initTank(p2.pos, p2.aimAngle, 2);
  drawPermText();

  wait = false;
}

void potentiometerWorker(void const *arg) {
  LPC_PINCON->PINSEL1 &= ~(0x03 << 18);
  LPC_PINCON->PINSEL1 |= (0x01 << 18);

  LPC_SC->PCONP |= (0x01 << 12);

  LPC_ADC->ADCR = (1 << 2) | // select AD0.2 pin
                  (4 << 8) | // ADC clock is 25MHz/5
                  (1 << 21); // enable ADC
  while (1) {
    LPC_ADC->ADCR |= (0x01 << 24);
    while (~LPC_ADC->ADGDR & (0x01 << 31))
      ;
    uint16_t potValue = (LPC_ADC->ADGDR & (0xfff << 4)) >> 4;
    // cast firepower to 1 to MAX_FIREPOWER
    firepower = ((double)potValue) / 4096 * MAX_FIREPOWER + 1;
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
      changePos += 1;
      break;
      // down is counter clockwise (25)
    case 0x00800000:
      changeAim -= 3;
      break;
      // up is clockwise (23)
    case 0x02000000:
      changeAim += 3;
      break;
    case 0x04000000:
      changePos -= 1;
      break;
    default:
      delay = false;
      break;
    }
    if (delay && !wait) {
      if (isP1) {
        // update position and ensure no overlap
        updatePositionWithCheck(&p1, changePos, &terrain, &p2);
        updateAim(&p1, -changeAim + p1.aimAngle);
      } else {
        updatePositionWithCheck(&p2, changePos, &terrain, &p1);
        updateAim(&p2, changeAim + p2.aimAngle);
      }
      // reduce sensitivity of joystick
      osDelay(1250);
    }
  }
}

void pushbuttonWorker(void const *arg) {
  bool lastButtonState = false;
  while (true) {
    // check if button is held
    if ((~LPC_GPIO2->FIOPIN & (0x01 << 10)) != lastButtonState && !lastButtonState) {
      lastButtonState = true;
      if (newGame) {
        // reset game
        setupGame();
        newGame = false;
      } else {
        // signal semaphore so game worker can do its thing
        if (!wait)
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
    // block peripherals from updating variables
    wait = true;
    printf("Firepower: %d\n", firepower);
    // fire updates the shot position, hence graphics will show new pos
    collided = fire(isP1 ? &p1 : &p2, isP1 ? &p2 : &p1, &ball, &terrain, firepower);
    if (collided) {
      // update terrain
      damage(&terrain, &ball);
      // update health and position
      updateStatus(&p1, &terrain, &ball);
      updateStatus(&p2, &terrain, &ball);
      // wait for explosion animation to finish
      osSemaphoreWait(graphics, osWaitForever);
    }
    // printTerrain(&terrain);
    // check if game ends
    if (p1.HP <= 0 || p2.HP <= 0) {
      newGame = true;
      // 3 for tie, 1 for p1 win, 2 for p2 win
      int gameResult = ((p1.HP <= 0) << 1) | (p2.HP <= 0);
      displayEndGame(gameResult);
      printf("Game Ended\n");
    } else {
      // switch turn
      isP1 = !isP1;
      // move ball out of sight
      hideShot(&ball);
      printf("Turn ended\n");
    }
    wait = false;
    osThreadYield();
  }
}

void graphicsWorker(void const *arg) {
  while (true) {
    if (!newGame) {
      if (collided) {
        // explosion animation
        impact(ball, &terrain);
        collided = false;
        // signal game worker to continue after explosion animation
        printf("RELEASING\n");
        osSemaphoreRelease(graphics);
      }
      updateGraphics(&p1, true);
      updateGraphics(&p2, false);
      // TODO: should just pass in coordinate
      updateShot(ball.x * 4, ball.y * 4);
      updatePowerBar(firepower);
      // osDelay(300);
    }
  }
}

osThreadDef(potentiometerWorker, osPriorityNormal, 1, 0);
osThreadDef(joystickWorker, osPriorityNormal, 1, 0);
osThreadDef(pushbuttonWorker, osPriorityNormal, 1, 0);
// once semaphore is available run the animation and calculations
osThreadDef(gameWorker, osPriorityNormal, 1, 0);
osThreadDef(graphicsWorker, osPriorityNormal, 1, 0);

int main(void) {
  printf("Starting\n");
  osKernelInitialize();
  osKernelStart();
  initGraphics(BACKGROUND_COLOR, BACKGROUND_COLOR, Black, &terrain);
  setupGame();
  // init semaphores
  semaphore = osSemaphoreCreate(osSemaphore(semaphore), 0);
  graphics = osSemaphoreCreate(osSemaphore(graphics), 0);
  // start threads
  osThreadId t1 = osThreadCreate(osThread(potentiometerWorker), NULL);
  osThreadId t2 = osThreadCreate(osThread(joystickWorker), NULL);
  osThreadId t3 = osThreadCreate(osThread(pushbuttonWorker), NULL);
  osThreadId t4 = osThreadCreate(osThread(gameWorker), NULL);
  osThreadId t5 = osThreadCreate(osThread(graphicsWorker), NULL);
  // Continue that main thread forever
  while (1)
    ;
}

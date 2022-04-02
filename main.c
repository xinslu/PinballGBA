#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gba.h"
#include "images/trolled.h"
#include "images/garbage.h"
#include "images/title.h"
#include "images/win.h"
/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.

char *loose = "You loose.";
char one ='1';
int isFlipperUp = 0;
Ball b;
char lives[2];
char score[3];
enum gba_state state;
unsigned short int yellow = COLOR(31, 31, 0);
unsigned short int current;
int frameCounter = 0;

int main(void) {
   REG_DISPCNT = MODE3 | BG2_ENABLE;
   videoBuffer[1208] = 0x7fff;
   b.lives = 3;
   b.score = 000;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;
  state = START;
  // Load initial application state

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    waitForVBlank();
    switch (state) {
      case START:
        drawFullScreenImageDMA(title);
        drawString(80, 100, "PINBALL", WHITE);
        if (KEY_DOWN(BUTTON_START, currentButtons)) {
          waitForVBlank();
          drawRectDMA(0,0,240,160, BLACK);
          state = PLAY;
        }
        break;
      case PLAY:
        drawBall();
        drawMap();
        drawFlipper();
        drawObstacles();
        delay(20);
        play();
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          waitForVBlank();
          b.score = 0;
          b.lives= 3;
          drawRectDMA(0,0,240,160, BLACK);
          state = START;
        }
        break;
      case WIN:
        drawFullScreenImageDMA(win);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          waitForVBlank();
          b.score = 0;
          b.lives= 3;
          drawRectDMA(0,0,240,160, BLACK);
          state = START;
        }
        break;
      case LOSE:
        drawString(10,100,loose, WHITE);
        drawImageDMA(40, 65, TROLLED_WIDTH, TROLLED_HEIGHT, trolled);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          waitForVBlank();
          b.score = 0;
          b.lives= 3;
          drawRectDMA(0,0,240,160, BLACK);
          state = START;
        }
        break;
      case RESTART:
        drawBall();
        state = PLAY;
        delay(20);
        play();
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }
  UNUSED(previousButtons); // You can remove this once previousButtons is used
  return 0;
}

void drawMap(void) {
  drawRectDMA(140,10, 80, 10, CYAN);
  drawRectDMA(140,150, 80, 10, CYAN);
  drawRectDMA(130,10, 30, 10, CYAN);
  drawRectDMA(130,200, 30, 10, CYAN);
  drawRectDMA(120,10, 20, 10, CYAN);
  drawRectDMA(120,210, 20, 10, CYAN);
  drawRectDMA(60,10, 10, 60, CYAN);
  drawRectDMA(60,220, 10, 60, CYAN);
  drawRectDMA(10,0, 240, 10, CYAN);
  drawRectDMA(20,0, 30, 10, CYAN);
  drawRectDMA(20,210,30, 10, CYAN);
  sprintf(lives, "%d", b.lives);
  drawString(1, 10, lives, WHITE);
  sprintf(score, "%d", b.score);
  drawString(1, 220, score, WHITE);
}

void drawFlipper(void) {
  for (int i = 0; i <= 25; i++) {
    drawRectDMA(140,90+i, 1, 3, YELLOW);
  }
  for (int i = 0; i <= 25; i++) {
    drawRectDMA(140,125+i, 1, 3, YELLOW);
  }
}

void drawObstacles(void) {
  // Bottom Left
  drawRectDMA(80,50, 25, 5, CYAN);
  drawRectDMA(100,50, 25, 5, CYAN);
  drawRectDMA(85,50, 5, 15, CYAN);
  drawRectDMA(85,70, 5, 15, CYAN);
  drawChar(89, 59, one, WHITE);

  // Bottom Right
  drawRectDMA(80,165, 25, 5, CYAN);
  drawRectDMA(100,165, 25, 5, CYAN);
  drawRectDMA(85,165, 5, 15, CYAN);
  drawRectDMA(85,185, 5, 15, CYAN);
  drawChar(89, 174, one, WHITE);

  // Top Middle
  drawRectDMA(35,108, 25, 5, CYAN);
  drawRectDMA(55,108, 25, 5, CYAN);
  drawRectDMA(40,108, 5, 15, CYAN);
  drawRectDMA(40,128, 5, 15, CYAN);
  drawChar(44, 117, one, WHITE);

  // Single Bar in Middle
  drawRectDMA(77,98, 45, 5, CYAN);

  // Bumper Left
  drawRectDMA(60,59, 8, 8, WHITE);

  // Bumper Right
  drawRectDMA(60,173, 8, 8, WHITE);
}

void drawBall(void) {
  b.col = randint(232,234);
  b.row = 150;
  drawRectDMA(150,b.col, 3, 3, WHITE);
}


void play(void) {
  int ry = -1;
  int cx = -1;
  while (state == PLAY) {
    if (b.score >= 50) {
      drawRectDMA(0,0,240,160, BLACK);
      state = WIN;
    }
    current = getPixel(b.row,b.col);
    u32 buttons = BUTTONS;
    if (KEY_DOWN(BUTTON_A, buttons)) {
          waitForVBlank();
          drawRectDMA(0,0,240,160, BLACK);
          state = WIN;
    }
    if (KEY_DOWN(BUTTON_SELECT, buttons)) {
          waitForVBlank();
          b.score = 0;
          b.lives= 3;
          drawRectDMA(0,0,240,160, BLACK);
          state = START;
    }
    if (KEY_DOWN(BUTTON_UP, buttons) && isFlipperUp != 1) {
      isFlipperUp = 1;
      drawRectDMA(140,90, 26, 3, BLACK);
      drawRectDMA(140,125, 26, 3, BLACK);
      for (int i = 0; i <= 25; i++) {
        drawRectDMA(140-i,90+i, 1, 3, YELLOW);
      }
      for (int i = 0; i <= 25; i++) {
        drawRectDMA(140-i,125+(25-i), 1, 3, YELLOW);
      }
    }
    if (isFlipperUp == 1) {
      frameCounter++;
    }
    if (frameCounter == 10) {
      for (int i = 0; i <= 25; i++) {
        drawRectDMA(140-i,90+i, 1, 3, BLACK);
      }
      // drawRectDMA(140,125, 26, 3, BLACK);
      for (int i = 0; i <= 25; i++) {
        drawRectDMA(140-i,125+(25-i), 1, 3, BLACK);
      }
      for (int i = 0; i <= 25; i++) {
        drawRectDMA(140,90+i, 1, 3, YELLOW);
      }
      for (int i = 0; i <= 25; i++) {
        drawRectDMA(140,125+i, 1, 3, YELLOW);
      }
      isFlipperUp = 0;
      frameCounter = 0;
    }
    if (current == yellow){
      ry=-ry;
      cx=-cx;
    } else if (getPixel(b.row+3,b.col) == yellow || getPixel(b.row-3,b.col) == yellow) {
      ry=-ry;
    }
    else if (getPixel(b.row,b.col+3) == yellow || getPixel(b.row,b.col-3) == yellow) {
      cx=-cx;
    }
    else if (getPixel(b.row+3,b.col+3) == yellow || getPixel(b.row-3,b.col-3) == yellow || getPixel(b.row+3,b.col-3) == yellow || getPixel(b.row-3,b.col+3) == yellow) {
      cx=-cx;
      ry=-ry;
    }
    else if (b.row > 160) {
        b.lives--;
        if (b.lives > 0) {
          updateText();
          state = RESTART;
        } else {
          waitForVBlank();
          drawRectDMA(0,0,240,160, BLACK);
          state = LOSE;
        }
    }

    // Left Outside
    else if (b.col < 3) {
      cx=-cx;
    }
    // Right Outside
    else if (compareX(237,240)) {
      cx=-cx;
    }

    // Bottom Two obstacles Rows
    else if ((compareY(77,80) || (compareY(105,105))) && (compareX(50,75) || compareX(165,190))) {
      ry=-ry;
      b.score++;
      updateText();
    }

    // Bottom Two Obtacles Column
    else if ((compareX(47,50) || compareX(163,165) || compareX(75,78) || compareX(190,192)) && compareY(80,105)) {
      cx=-cx;
      b.score++;
      updateText();
    }

    // Top Middle Obstacle Col
    else if((compareX(106,108) || compareX(130,133)) && (compareY(35,60))) {
      cx=-cx;
      b.score++;
      updateText();
    }

    // Middle Bar Col
    else if ((compareX(96,98) || compareX(143,144)) && (compareY(77,82))) {
      cx=-cx;
    }

    //Top Middle Obstacle Rows
    else if ((compareY(33,35) || compareY(60,62)) && (compareX(108,108+25))) {
      ry=-ry;
      b.score++;
      updateText();
    }

    // Middle Bar Row
    else if ((compareY(74,77) || compareY(82,83)) && (compareX(97,99+45))) {
      ry=-ry;
    }

    // Bumpers Columns
    else if ((compareX(56,59) || compareX(66,68) || compareX(172,177) || compareX(181,183)) && (compareY(60,68))) {
      cx=-cx;
    }

    // Bumpers Rows
    else if ((compareY(58,60) || compareY(70,72)) && (compareX(59,67) || compareX(172,172+8))) {
      ry=-ry;
    }

    //Second Top Sidebars and Thrid Low bars Col
    else if ((compareX(30,32) || compareX(207,210)) && (compareY(20,29) || compareY(119,131))) {
      cx=-cx;
    }

    // Second Low Bar Col
    else if ((compareX(40,43) || compareX(197,200)) && (compareY(129,141))) {
      cx=-cx;
    }
    // Second Low Bar Row
    else if ((compareY(128,130)) && (compareX(9,42) || compareX(200,230))) {
      ry=-ry;
    }

    else if ((compareY(138,140)) && (compareX(10,90) || compareX(150,200))) {
      ry=-ry;
    }
    else if (compareY(20,21) && (compareX(0,240))) {
      ry=-ry;
    }
    else if (compareY(30,31) && (compareX(210, 240) || compareX(0,30))) {
      ry=-ry;
    }
    else if (compareY(117,120) && (compareX(210, 229) || compareX(10,30))) {
      ry=-ry;
    }
    // Two Side Pillars
    else if ((b.col == 230 || compareX(18,20) || compareX(218,220) || compareX(8,10)) && compareY(58,150)) {
      cx=-cx;
    }
    else if (compareY(57,60) && (compareX(10,20) || compareX(220,230))) {
      ry=-ry;
    }
    drawRectDMA(b.row, b.col, 3, 3, BLACK);
    b.row = b.row + ry;
    b.col = b.col + cx;
    drawRectDMA(b.row, b.col, 3, 3, WHITE);
    delay(1);
  }
}

void delay(int n) {
  volatile int x = 0;
  for(int i=0; i<n*8000; i++) {
    x++;
  }
}


void updateText(void) {
  drawRectDMA(0,0,240,10, BLACK);
  sprintf(lives, "%d", b.lives);
  drawString(1, 10, lives, WHITE);
  sprintf(score, "%d", b.score);
  drawString(1, 220, score, WHITE);
}

int compareX(int xstart, int xend) {
  return (b.col >= xstart) && (b.col <= xend);
}

int compareY(int ystart, int yend) {
  return (b.row >= ystart) && (b.row <= yend);
}

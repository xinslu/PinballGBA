#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/
void drawMap(void);
void drawFlipper(void);
void drawObstacles(void);
void drawBall(void);
double *rotate_point(int pointX, int pointY, int originX, int originY, double angle);
void play(void);
void delay(int n);
int compareX(int xstart, int xend);
int compareY(int ystart, int yend);
void updateText(void);
enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
  RESTART
};



typedef struct ball {
    int row;
    int col;
    int lives;
    int score;
} Ball;

#endif

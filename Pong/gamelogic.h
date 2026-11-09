/**
 * Header file for gamelogic.c
 * Written by Seerat Yasir, Chloe Wabingga 
 * Last updated: 2023-12-08
*/

#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <stdint.h> 
#include <pic32mx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>  
#include "globalvars.h"

/* 75 degrees in radian*/
#define MAXTRAJECTORYANGLE 1.30899694 
#define RACKET_HEIGHT 12
#define PI 3.14159265359

/* Game speed and difficulty*/
#define EASY_SPEED 5
#define MEDIUM_SPEED 4
#define HARD_SPEED 3
int speed;

/* Player 1 */
int racket1_X;
int racket1_Y;
/* Player 2 or AI*/
int racket2_X;
int racket2_Y;

/* Ball coordinates */
int ball_X;
float ball_Y;
/* Ball speed */
int speed_X;
float speed_Y;

void play_game(int gamemode, int difficulty);
void init_game(void);
void multiplayergame(int speed);
void singleplayergame(int difficulty);
void ai_move_mode(int speed, int mode);
void ai_move(int mode);
void move_racket(int racket, int direction);
int ball_move(void);
float speed_Y_update(int racket);
void update_scoreboard(void);
void pause_game(void);

int max(int a, int b);
int count;

#endif


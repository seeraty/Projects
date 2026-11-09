/**
 * Header file for global variables. 
 * Written by Chloe W, Seerat Y
*/
#include <stdint.h> 
#include <pic32mx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>  

#ifndef GLOBALVARS_H
#define GLOBALVARS_H

void portsinit(void);

int btn1(void);
int btn2(void);
int btn3(void);
int btn4(void);
int sw1(void);
int sw4(void);

/* Player structure */
struct player
{
    int score;
    char name;
};

// Create player variables
struct player player1;
struct player player2;
struct player winner;


#endif  

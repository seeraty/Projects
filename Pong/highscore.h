/**
 * Header file for highscore.c
 * Written by Chloe W, Seerat Y
*/
#ifndef HIGHSCORE_H_INCLUDED
#define HIGHSCORE_H_INCLUDED
#include <stdint.h> 
#include <pic32mx.h>
#include "globalvars.h"

void display_highscore_menu();
void set_initials();
void set_new_highscore();
void set_initials1();
void check_score();
void highscore_init();
void update_score(int rank);
void highscore_menu();

struct highscore
{
    char name;
    int score;
};

char initials[3] = "";
char letter[26] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
//char* letter =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
struct highscore highscore_list[10];

#endif
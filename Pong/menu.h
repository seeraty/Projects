/**
 *  Header file for menu.c 
 *  Written by Chloe Wabingga, Seerat Yasir
 */

#include <stdint.h>
#include <pic32mx.h> 

int gamemode; // 0 = singleplayer (vs AI), 1 = multiplayer 
int difficulty; // 0 = easy, 1 = medium, 2 = hard

//void display_start_screen(void);
int navigate_main_screen(int gamemode, int difficulty);
void choose_mode(int gamemode, int difficulty);
void play_game(int gamemode, int difficulty);
void display_black_screen(void);

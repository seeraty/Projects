/**
 * Functions related to displaying and choosing from menu screens.
 * Written by Seerat Yasir, Chloe Wabingga
 * Last edited: 2023-12-08
*/

#include "globalvars.h"
#include "menu.h"

/** 
 * Navigate the main screen.
 * BTN1 is used to confirm a selection.
 * BTN4 is used to navigate between options.
*/
int navigate_main_screen(int gamemode, int difficulty) {
    display_clear();
    LED_off();
    int option = 0;  

    while (btn1() == 0) {    
        if (btn4() == 1) {
            option++;
            option = option % 4;
            delay(250);
            display_clear();
        }
        
        display_string(1, 1, "Play");
        display_string(1, 13, "Modes");
        display_string(1, 23, "Highscores");
        display_string(95, 1, "Exit");

        if (option == 0) {  // Light up right option by underlining it
            display_line(1, 32, 9);
        } else if (option == 1) {  
            display_line(1, 42, 21);
        } else if (option == 2) {  
            display_line(1, 79, 31);
        }  else if (option == 3) {
            display_line(95, 127, 9);
        }
        
        display_update();
    }
    
    /* Play the game */
    if (option == 0) {
        delay(200);
        display_clear();
        play_game(gamemode, difficulty);
    } 
    /* Choose gamemodes and difficulty*/
    else if (option == 1) {
        delay(200);
        display_clear();
        choose_mode(gamemode, difficulty);
    } 
    /* Check the highscore menu*/
    else if (option == 2) {
        delay(200);
        display_clear();
        highscore_menu();
    } 
    /* 'Close' the game */
    else if (option == 3)
    {
        delay(200);
        display_black_screen();
    }
    
    
}

/**
 * Menu for choosing mode and difficulty
*/
void choose_mode(int gamemode, int difficulty) {
    while (btn1() == 0) {    
        if (btn4() == 1) {
            gamemode++;
            /* 1 = multiplayer, 2 = Singleplayer*/
            gamemode = gamemode % 2; 
            delay(250);
            display_clear();
        }

        display_string(1, 1, "Gamemode: ");
        display_string(1, 12, "Two players");
        display_string(1, 23, "Player vs AI");

        if (gamemode == 0) {  
            display_line(1, 97, 31);
        } else if (gamemode == 1) {  
            display_line(1, 90, 20);
        }

        display_update();
    } 

    delay(500);
    display_clear();

    if (gamemode == 0)
    {   
        while (btn1() == 0) {    
            if (btn4() == 1) {
                difficulty++;
                difficulty = difficulty % 3; 
                delay(250);
                display_clear();
            }

            display_string(1, 1, "Difficulty level: ");
            display_string(1, 13, "Easy");
            display_string(64,16, "Medium");
            display_string(1, 23, "Hard");

            if (difficulty == 0) {  // light up right option by underlining it
                display_line(2, 30, 21);
            } else if (difficulty == 1) {  
                display_line(64, 115, 24);
            } else if (difficulty == 2) {
                display_line(2, 30, 31);                
            }
        }
    }

    delay(250);
    navigate_main_screen(gamemode,difficulty);
}

/* Clear display to imitate a black offline screen */
void display_black_screen() {
    display_clear();

    while (1)
    {
        if (btnclick() == 1)
        {
            delay(300);
            start_anim();
        }
    }
    
}


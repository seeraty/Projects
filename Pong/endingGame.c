/**
 * Functions for when a game has ended and scores need to be checked
 * Written by Chloe W, Seerat Y
 * Updated: 2023-12-08
*/

#include "globalvars.h"
#include "menu.h"

void endgame_multiplayer(int player, int score);
void endgame_singleplayer(int winner, int score);

/* Ending for multiplayer*/
void endgame_multiplayer(int player, int score) {
    display_clear();
    char winnerText[50];
    char scoreToDisplay[50];

    sprintf(winnerText, "Player %d won!", player);  // sprintf stores string (and ints) on a char array. 
    sprintf(scoreToDisplay, "Score: %d", score);

    display_string(35, 16, "Game over!"); // Adjust so it is displayed in the middle
    delay(2000);

    display_clear();
    display_string(1, 1, winnerText);
    display_string(1, 12, scoreToDisplay);

    while (btnclick() == 0 )
    {
        winner_LED();
    }

    check_score();
}

/* Ending for singleplayer*/
void endgame_singleplayer(int player, int score) {
    display_clear();
    char scoreToDisplay[50];
    
    display_string(32, 16, "Game over!"); // Adjust so it is displayed in the middle

    delay(2000);

    display_clear();
    if (player == 2) { 
        display_string(32, 16, "You lost :(");
    
        delay(2000);
        display_clear();
        display_string(1,1, "Try again?");

        while (1)
        {
            display_string(1,20,"BTN4: N  BTN1:Y");
            if (btn1() == 1)
            {
                delay(100);
                play_game(gamemode, difficulty);
            }

            if (btn4() == 1)
            {
                delay(100);

                display_clear();
                sprintf(scoreToDisplay, "Score: %d", score);
                display_string(13, 16, scoreToDisplay);

                while (btnclick() == 0)
                {    
                    LED_on();
                }

                check_score();
            }
        }
    } 
    else {

        while (btnclick() == 0)
        {
            display_string(32,16,"You won!!"); 
            winner_LED();
        }
        
        delay(250);

        display_clear();
        sprintf(scoreToDisplay, "Score: %d", score);
        display_string(13, 16, scoreToDisplay);

        while (btnclick() == 0)
        {    
            LED_on();
        }
        
        check_score();
    }
}


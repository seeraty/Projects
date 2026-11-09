/**
 * Functions related to displaying and storing highscores of players.
 * Written by Chloe Wabingga, Seerat Yasir
 * Modified 2023-12-08
*/

#include "highscore.h"
#include "menu.h"


/* Called after a game has ended */
void check_score(){
    int rank = 0;

    for(rank = 0; rank < 10; rank++){
        if (winner.score >= highscore_list[rank].score)
        {
            delay(250);
            display_clear();
            display_string(16,16, "New highscore!");
            
            while (btnclick() == 0)
            {
                winner_LED();
            }
            
            LED_off();
            set_initials();
            update_score(rank);
            break;
        }
    }

    navigate_main_screen(gamemode,difficulty);
        
}

/**
 * Set initials to a highscore
 * BUG: Might show a random symbol next to the letters when scrolling through the alphabet
*/
void set_initials() {
    display_clear();
    strcpy(&winner.name,"");
    strcpy(initials,"");

    display_string(1,1, "Initials:");
    display_line(79,83,9); 
    display_line(86,90,9);
    display_line(93,97,9);  
    display_update();

    int i = 0;
    int j = 0;
    int x = 70;

    while (j < 3)
        {
            char c = letter[i];
            display_string(1,20,&c);
            display_update();

            /* Next letter */
            if (btn4() == 1)
            {   
                if (i == 25)
                {
                    i = -1;
                }
                
                display_clear_page(4);
                display_clear_page(3);
                i++;
                c = letter[i];
                delay(250);
            }
            
            /* Previous letter */
            if (btn3() == 1)
            {
                if (i == 0)
                {
                    i = 26;
                }
                display_clear_page(4);
                display_clear_page(3);
                i--;
                c = letter[i];
                delay(250);
            }

            /* Confirm */
            if (btn1() == 1)
            {
                strncat(initials, &c, 1);
                j++;
                x = x + 7;
                display_string(x,1,&c);
                display_update();
                delay(250);
            }

        }

        /* Confirmation screen */
        strcpy(&winner.name,initials);
        display_clear();
        display_string(1,1,"Confirm");
        display_string(61,1, &winner.name); 
        display_string(86,1, "?"); 
        display_string(1,20, "BTN4:N BTN1:Y");
        display_update(); 

        while (1)
        {    
            if (btn4() == 1)
            {
                delay(500);
                set_initials();
            }
            else if (btn1() == 1)
            {
                return;
            }
        } 
}

/* Update the highscore list*/
void update_score(int rank){
    display_clear();
    int i;
    for (i = 9; i > rank; i--)
    {
        highscore_list[i].score = highscore_list[i-1].score;
        strcpy(&highscore_list[i].name,&highscore_list[i].name);
    } 

    strcpy(&highscore_list[rank].name, &winner.name);
    highscore_list[rank].score = winner.score;

    highscore_menu();
    
}

/* Initialize highscore list */
void highscore_init(){
    highscore_list[0].score = 0;
    highscore_list[1].score = 0;
    highscore_list[2].score = 0;
    highscore_list[3].score = 0;
    highscore_list[4].score = 0;
    highscore_list[5].score = 0;
    highscore_list[6].score = 0;
    highscore_list[7].score = 0;
    highscore_list[8].score = 0;
    highscore_list[9].score = 0; 
}

/* Highscore menu */
void highscore_menu(){
    int i = 0;

    while (btn1() == 0)
    {
        char score_str[100];
        char rank[1];

        /* Scroll*/
        if (btn3() == 1 && i > 0)
        {
            i--;
            delay(250);
            display_clear();
        }
        if (btn4() == 1 && i < 9)
        {
            i++;
            delay(250);
            display_clear();
        }

        /* Display score */
        display_string(1,1,"Highscore");
        sprintf(rank,"%d",i+1);
        strncat(rank,":",1);
        display_string(73,1,rank);


        if (highscore_list[i].name == '\0')
        {
            display_string(1,12,"No highscore");
            display_update();
            
        } 
        else
        {
            display_string(1,12,&highscore_list[i].name);
            display_string(23,12,",");
            sprintf(score_str,"%d",highscore_list[i].score);
            display_string(30,12,score_str);
            display_update();
        }
    }

    if (btn1() == 1)
    {   
        delay(250);
        navigate_main_screen(gamemode,difficulty);
    }
}

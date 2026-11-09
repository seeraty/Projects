/**
 *  gamelogic.
 *  Core logic of the ping pong game that is unrelated to the ChipKit UNO32 and other hardwares.
 *  Written by Seerat Yasir, Chloe Wabingga 
 *  Last updated: 2023-12-09
*/
#include "globalvars.h"
#include "gamelogic.h"

/* Start game based on settings. Called from menu.c */
void play_game(int gamemode, int difficulty) {
    if(difficulty == 0) {   // Easy
        speed = EASY_SPEED;
    } else if(difficulty == 1) {    // Medium
        speed = MEDIUM_SPEED;
    } else if(difficulty == 2) {    // Hard
        speed = HARD_SPEED;
    }

    if (gamemode == 0) {
        singleplayergame(speed);
    } else if (gamemode == 1) {
        multiplayergame(speed);
    }
}

/* Initialize game */
void init_game(){
    display_clear();

    // Players
    strcpy(&player1.name,"");
    player1.score = 0;
    strcpy(&player2.name,"");
    player2.score = 0;
    strcpy(&winner.name,"");
    winner.score = 0;
    
    // Racket starting values
    racket1_X = 1;
    racket1_Y = (32-RACKET_HEIGHT)/2;
    racket2_X = 127;
    racket2_Y = (32-RACKET_HEIGHT)/2;

    // Ball starting values 
    ball_X = 64;
    ball_Y = 16;
    speed_X = -1;  // Ball always goes to player 1 at the start
    speed_Y = 0; // Ball goes straight in the beginning

    count_down();

    display_vertical(racket1_Y, racket1_Y + RACKET_HEIGHT, racket1_X);
    display_vertical(racket2_Y, racket2_Y + RACKET_HEIGHT, racket2_X);
    display_ball(ball_X, (int)ball_Y);
    update_scoreboard();
}

/* Function for multiplayer game. */
void multiplayergame(int speed) {
    init_game();
    int playerwinner = 0;
	update_scoreboard();

    /* Game loop until a winner has been declared */
    while(playerwinner == 0) {
        if(sw1() == 1) {
            pause_game();
        } 
        playerwinner = ball_move();    

        /* Player 1*/
	    if (btn4() == 1) {
	        move_racket(1, 1);
        }
        if (btn3() == 1) {
            move_racket(1, -1);
        }

        /* Player 2*/
        if (btn2() == 1) {
            move_racket(2, 1);
        }
        if (btn1() == 1) {
            move_racket(2, -1);
        }
        delay(speed);
    }

    /* Get the highest score, will always be the winning player */
    int highestScore = max(player1.score, player2.score); 
    winner.score = highestScore;
    endgame_multiplayer(playerwinner, highestScore);
}   

/* Singleplayer game */
void singleplayergame(int speed) {
    init_game();
    int mode = 1;
    int playerwinner = 0;
        update_scoreboard();

    /* Game loop until winner has been declared*/
    while(playerwinner == 0) {
	    if(sw1() == 1) {
            pause_game();
        } 
        playerwinner = ball_move();   

        /* Player 1*/
        if (btn4() == 1) {
            move_racket(1, 1);
        }
        if (btn3() == 1) {
            move_racket(1, -1);
        }
            ai_move_mode(speed, mode);
        
        delay(speed);  
    }     

    winner.score = player1.score;
    endgame_singleplayer(playerwinner, winner.score);
} 

/* Moves racket one pixel in desired direction */
void move_racket(int racket, int direction) {
    int x, y, full_racket;
    
    /* Move racket up by one pixel*/
    if (direction == 1)
    {
        /* Left racket */
        if (racket == 1 && racket1_Y > 1 )
        {
            x = racket1_X;
            y = racket1_Y;
            full_racket = racket1_Y + RACKET_HEIGHT;
            racket1_Y--;
            display_clear_pixel(x, full_racket);
            display_vertical(y - 1, full_racket - 1, x);
        }

        /* Right racket*/
        if (racket == 2 && racket2_Y > 1)
        {
            x = racket2_X;
            y = racket2_Y;
            full_racket = racket2_Y + RACKET_HEIGHT;
            racket2_Y--;
            display_clear_pixel(x, full_racket);
            display_vertical(y - 1, full_racket - 1, x);
        }
    }

    /* Move racket down by one pixel */
    if (direction == -1 )
    {
        if (racket == 1 && racket1_Y + RACKET_HEIGHT < 32)
        {
            x = racket1_X;
            y = racket1_Y;
            full_racket = racket1_Y + RACKET_HEIGHT;
            racket1_Y++;
            display_clear_pixel(x, y);
            display_vertical(y + 1, full_racket + 1, x);
        }
        /* Right racket*/
        if (racket == 2 && racket2_Y + RACKET_HEIGHT < 32)
        {
            x = racket2_X;
            y = racket2_Y;
            full_racket = racket2_Y + RACKET_HEIGHT;
            racket2_Y++;
            display_clear_pixel(x, y);
            display_vertical(y + 1, full_racket + 1, x);
        }
    }   
}


/**
 * Returns 0 if ball is in game, 1 if player 2 misses and 2 if player 1 misses ball.
 * Increments player score otherwise.
*/
int ball_move(void) {
    float angle;

    clear_ball(ball_X, (int)ball_Y);
    /* If ball hits the scoreboard, part of the scoreboard is cleared with the ball. Thus, we need to update it. */
    if (ball_Y <= 7 && ball_X >= 43 && ball_X <= 84) {
        update_scoreboard();
    }

    /* Ball hits left racket */
    if (ball_X == 2 || ball_X == 1) {   
        if (check_if_pixel_is_lit(1, (int)ball_Y) == 1 || check_if_pixel_is_lit(1, (int)ball_Y + 1) == 1) { // Because ball is 2x2 
            speed_X *= -1;
            speed_Y = speed_Y_update(1);
            player1.score++;
            update_scoreboard();
        } else { 
            /* player 1 misses ball and player 2 / AI wins */
            return 2; 
        } 
    } 
    /* Ball hits right racket or it's edge from y direction */
    else if (ball_X == 125 || ball_X == 126) { 
        if (check_if_pixel_is_lit(127, (int)ball_Y) == 1 || check_if_pixel_is_lit(127, (int)ball_Y + 1) == 1) {  
            speed_X *= -1;
            speed_Y = speed_Y_update(2);
            player2.score++;
            update_scoreboard();
        } else {    
            /* player 2 misses ball and player 1 wins */
            return 1; 
        }
    } 

    /* Ball hits upper or lower wall - it will bounce/mirror movement */
    if (ball_Y <= 1 || ball_Y >= 32) { // Var >= 30
        speed_Y = -(speed_Y);
    }
   
    /* New ball coordinates */    
    ball_X += speed_X;
    ball_Y += speed_Y;

    display_ball(ball_X, (int)ball_Y);
    return 0;
}

/**
 *   Returns a float of y's new movement per pixel movement in x-axis. 
 *   Value/direction is based on how far from the center of the paddle the ball hits.
 *   The further away, the more extreme the angle.
 */
float speed_Y_update(int racket) {
    float racketY;
    if (racket == 1)
    {
        racketY = racket1_Y;
    } else {
        racketY = racket2_Y;
    }
    
    /* Calculate how far from the center*/
    float centerOffset = (racketY + (float)(RACKET_HEIGHT/2) - 1) - ball_Y;
    /* Normalize, range [-1, 1] where -1 is the bottom of the racket, 0 the center, and 1 the top. */
    float normalizedOffset = centerOffset/(float)(RACKET_HEIGHT/2);
    /* Bounce angle. Think of enhetscirkeln */
    float bounceAngle = normalizedOffset * (float)MAXTRAJECTORYANGLE; // Maxtraj är -90 till 90 grader
    
    /* Motsvarande y and x coordinates from the bounce when hypothenusa = 1 */
    float y_prem = -sin(bounceAngle); 
    float x_prem = cos(bounceAngle);

    /* Calculate movement in y when x moves 1 pixel */
    float x_rest = 1 / x_prem; // x förlängningsfaktor för att få x = 1
    float y_correct = y_prem*x_rest; // because of sides being divisible

    /* Round up / down */
    if (y_correct > 0)
    {
        y_correct = ceil(y_correct); 
    }

    if (y_correct < 0)
    {
        y_correct = floor(y_correct);
    } 
    return y_correct;
}


/* Update the scoreboard by one point */
void update_scoreboard() {
    clear_scoreboard();
    char scoreboard[3];
    
    sprintf(scoreboard, "%d:%d", player1.score, player2.score);
    
    if(player1.score < 10) {
        display_string(62, 1, scoreboard);
    } else {
        display_string(58, 1, scoreboard);
    }    
}

/* Math helper functions */
int max(int a, int b) {
    if(a >= b) {
        return a;
    } else if (a < b) {
        return b;
    }
}

/* Pauses the game */
void pause_game(void) {
    display_clear();
    LED_on();
    display_string(62, 16, "Game Paused"); 
    
    while(sw1() == 1) {
        // do nothing but endlessly loop
    }

    LED_off();
    display_vertical(racket1_Y, racket1_Y + RACKET_HEIGHT, racket1_X);
    display_vertical(racket2_Y, racket2_Y + RACKET_HEIGHT, racket2_X);
    display_ball(ball_X, ball_Y);
    update_scoreboard();
    
}

/**
 *  Unsure if AI should be unbeatable or not. Two modes for the player to choose
 *  0 = beatable AI, 1 = unbeatable AI
 *  If beatable AI, make the the player 2 racket move in a frequency based on the difficulty.
 *  The higher difficulty, the more often it moves, and thus the higher chance it hits the ball. 
 * */
void ai_move_mode(int speed, int mode) {
    if (mode == 0) {  
        if (ball_X % speed == 0) { // alt ändra mod eft speeds är för nära för satt ai ska bli smartare
            ai_move(mode);
        }
    } else if(mode == 1) { 
        ai_move(mode); 
    }   
}

/* 
 * AI movement. Is called less or more frequently depending on the difficulty of the game,
 * and moves up if the balls location is over the racket and down if the ball is lower.
 * Remains it's position if the ball is on eye-level to the racket.   
 */
void ai_move(int mode) {
    int ball_Y_position;

    if (mode == 0) {
        ball_Y_position = (int)ball_Y;
    } else {
        ball_Y_position = (int)(ball_Y + speed_Y);   // predict ball's next move
    }

    if (ball_Y_position < racket2_Y ) {
        move_racket(2, 1);
    }
    
    if (ball_Y_position - 1 > racket2_Y + RACKET_HEIGHT) { 
        move_racket(2, -1);
    } // else remain position
}

/**
 * Functions for display of the start screen 
*/


#include "globalvars.h"

void start_anim(void);
void racket(void);
void border_anim(void);
void gameover_LED(void);

void start_anim(){    
    delay(200);
    display_string(30,11,"P");
    display_string(40,11,"O");
    display_string(50,11,"N");
    display_string(60,11,"G"); 
    racket();    
    border_anim();
    delay(500);

    while (1)
    {
        if (btn1() == 1 || btn2() == 1 || btn3() == 1 || btn4() == 1)
        {
            delay(200);
            navigate_main_screen(1,0);
        }
    }
}

/* Border around screen animation */
void border_anim(){
    int x1,x2,y1,y2;
    for (y1 = 0; y1 < 32; y1++)
    {
        display_pixel(1, y1);
        delay(3);
        display_update();
    }

    for (x1 = 0; x1 < 127; x1++)
    {
        display_pixel(x1,31);
        delay(3);
        display_update();
    }
    
    for (y2 = 31; y2 > 0 ; y2--)
    {
        display_pixel(127, y2);
        delay(3);
        display_update();
    }

    for (x2 = 127; x2 > 0; x2--)
    {
        display_pixel(x2,1);
        delay(3);
        display_update();
    }
}

/**
 * Very unconventional method of drawing a racket on the display. 
 * Much better way would be to create a function that converts an image file (BMP?) 
 * to an array.
 * */
void racket(){
    display_line(78,83,6);
    display_line(78,83,18);
    display_vertical(10, 15, 73);
    display_vertical(10, 15, 87); 
    display_pixel(74,9);
    display_pixel(75,8);
    display_pixel(76,7);
    display_pixel(77,7);
    display_pixel(83,7);
    display_pixel(84,7);
    display_pixel(85,8);
    display_pixel(86,9);
    display_pixel(74,15);
    display_pixel(75,16);
    display_pixel(76,17);
    display_pixel(77,17);
    display_pixel(86,15);
    display_pixel(85,16);
    display_pixel(84,17);
    display_pixel(83,17);

    display_pixel(84,18);
    display_pixel(85,19);
    display_pixel(86,19);
    display_pixel(87,20);
    display_pixel(88,21);
    display_pixel(89,22);
    display_pixel(90,22);
    display_pixel(91,23);
    display_pixel(87,16);
    display_pixel(88,17);
    display_pixel(89,18);
    display_pixel(90,18);
    display_pixel(91,19);
    display_pixel(92,20);
    display_pixel(93,21);
    display_pixel(92,22);
    display_update();
}

void winner_LED(){
    PORTE = 0b01010101;
    delay(100);
    PORTE = 0b10101010;
    delay(100);
}

/* Why isn't TRISECLR working :(?*/
void LED_off(){
    PORTE = 0b00000000;
}

void LED_on(){
    PORTE = 0b11111111;
}

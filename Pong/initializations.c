/**
 * Initialization of ports, buttons, switches and LEDs
 * Written by Seerat Y, Chloe W
 * */

#include "globalvars.h"

/**SW1: Pause game. Uses RD8

BTN1: select / confirm / player2 up. Uses RF1
BTN2: next option / player2 down. Uses RD5
BTN3: nav left / player1 down. Uses RD6
BTN4: nav right / player1 up. Uses RD7
 */   


void portsinit(void)
{
    volatile unsigned int* trise = (volatile unsigned int*) 0xbf886100; // For LEDS
    *trise &= 0xFF00; 
    
    PORTF &= 0b10; // BTN1 is common for both modes and uses Port F bit 1, bit starts at 0?
    // For now, we are initializing all buttons. To be changed later, see oneplayer init and multiplayer init
    PORTD &= 0b100111100000; // Set switches and BTN 2 here and then AND it in multiplayer mode?
}

int btn1(void) {
    return (PORTF >> 1) & 0b1;
}

int btn2(void) {
    return (PORTD >> 5) & 0b1;
}

int btn3(void) {
    return (PORTD >> 6) & 0b1;
}

int btn4(void) {
    return (PORTD >> 7) & 0b1;
}

int sw1(void) {
    return (PORTD >> 8) & 0b1;
}

/* int sw4(void) {
  return (PORTD >> 11) & 0b1; // Read bit 12 of PORTD
}
 */

/* Returns 1 if any of the buttons are clicked*/
int btnclick (void){
    if (btn1() == 1 || btn2() == 1 || btn3() == 1 || btn4() == 1) {
        return 1;
    }
    
    return 0;
}

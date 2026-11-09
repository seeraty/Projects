#include <stdint.h> 
#include <pic32mx.h>
void *stdin, *stdout, *stderr;

/**
 * Should include initialization of buttons, clocks, timers etc....
 * Start up display....
*/

/* Low level initialization of microcontroller */
void init_mcu(void)
{
	/* Following template from mipslabmain.c in lab 1 to set the peripheral 
      bus clock to the same frequency as the sysclock. That means 80 MHz, when the microcontroller
    is running at 80 MHz.
    */
    SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */   // Oscillator Control Register, kontrollerar clock source switching och tillåter monitoring av clock sources.
    SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
    while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
    OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
    while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
    SYSKEY = 0x0;  /* Lock OSCCON */

	
	/* Set up peripheral bus clock */
	/* OSCCONbits.PBDIV = 1; */
	// OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	// OSCCONSET = 0x080000; /* set PBDIV bit 0 */
	
    
    /* Set up output pins */
    AD1PCFG = 0xFFFF;  // sets pins as analogue? 
    ODCE = 0x0;        
    TRISECLR = 0xFF;
    PORTE = 0x0;
    
    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

	/* Enable LED1 through LED8 */
	// TRISECLR = 0x00FF;
    
    /* Set up input pins */ // Already done? What is the difference to PORTD directly?
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    /* SPI2STAT bit SPIROV = 0; */
    SPI2STATCLR = 0x40;
    /* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
    /* SPI2CON bit MSTEN = 1; */
    SPI2CONSET = 0x20;
    /* SPI2CON bit ON = 1; */
    SPI2CONSET = 0x8000;

}

int main(void) {
    /* Initializations */
	init_mcu();
	display_init();
	display_update();
	portsinit();
    highscore_init();
    display_black_screen();
}

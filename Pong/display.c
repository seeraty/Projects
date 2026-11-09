/**
 * display.c
 *  Functions related to displaying on the basic I/O shield.
 *  Includes original code by Fredrik Lundeval and Axel Isaksson, see file COPYING
 * 
 *  Written by:
 * 		 		Chloe Wabingga, Seerat Yasir
 * Last modified: 2023-12-09
 *  
*/
#include "display.h"

/* 128 bytes x 4 pages */
static uint8_t screenArray[128][4];

/**
 * Initialize the display
 * Written by Fredrik Lundvall, Axel Isaksson
*/
void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

/**
 * Original code by Fredrik Lundvall & Axel Isaksson
 * Modified by Chloe Wabingga
*/
void display_update(void) {
	int curPage, curColumn, k, j, c;
	int byte;

	/* Iterate over the pages, column by column. Based on the pseudocode from the reference manual */
	for(curPage = 0; curPage < 4; curPage++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		/* Set the page address*/
		spi_send_recv(0x22);
		spi_send_recv(curPage);
		
		/* Start at the left column*/
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		/* Send byte to SPI */
		for(curColumn = 0; curColumn < 128; curColumn++) {
			byte = screenArray[curColumn][curPage];
			spi_send_recv(byte);
		} 	
	}	
}

/**
 * Display a line of string *s, 
 * Original code by Fredrik Lundvall & Axel Isaksson
 * Modified by Chloe Wabingga
*/
void display_string(int x, int y, char *s) {
	/* x: beginning of string */
	int i, chara, col, c, row, pixel;

	/* Empty string */
	if(!s)
		return;
	
	/* Put strings to the textbuffer array*/
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[i] = *s;
			s++;
		} else
			textbuffer[i] = ' ';

	/* Put string in screen array. Up to 16 characters per line. */
	for (chara = 0; chara < 16; chara++)
	{
		/* Currect character to display */
		c = textbuffer[chara];
		
		/* Each character displayed by an 8x8 pixel */
		for(col = 0; col < 8; col++) {

			/* Get the pixel-data on the column */
			pixel = font[c * 8 + col];

			for (row = 0; row < 8; row++)
			{
				/* Check if any pixels on the column should be turned on */
				if ((pixel >> row) & 0x1)
				{
					display_pixel(x+8*chara+col,y+row);
				}
			}
		}	
	}
	display_update();
}

/**
 * display_pixel - Helper method
 * BUG: can only go up to y = 31, x = 127, probably due to the -1 in code line 136-137?
*/
void display_pixel(int x1, int y1){
	/* Check if valid coordinates */
    if(x1 <= 0 || x1 >= 128 || y1 <= 0 || y1 >= 32)
        return;
	
	/* (1,1) is first row and first column, take 1 from x and y due to zero-indexing*/
	int x = x1 - 1;
	int y = y1 - 1;

	/* Setting the byte in the correct row page-wise is aided by the remainder of y % 8 */
	/* Example: y = 18 is the second row in page 3. (18 - 1) % 8 = 1. We need to shift the enable bit 1 spot to the left. */
	int bit_offset = y % 8;                 

	/* Get page number */
    int page  = y / 8; 

    /* Enable the correct bit in the array */
    screenArray[x][page] |= 0x1 << bit_offset;
}

/**
 * Displays 2x2 ball with the top-right corner on (x,y)
*/
void display_ball(int x, int y){
	display_pixel(x,y);
	display_pixel(x+1,y);
	display_pixel(x,y+1);
	display_pixel(x+1,y+1);

	display_update();
}

/* Remove the ball from display */
void clear_ball(int x, int y) {
	if(x > 1 && x < 127) { // Because ball is 2 pixels wide
		display_clear_pixel(x,y);
		display_clear_pixel(x+1,y);
		display_clear_pixel(x,y+1);
		display_clear_pixel(x+1,y+1);
	}

	display_update();
}

/* Clear the entire display screen */
void display_clear(void) {
	int page, column;
	
	for (page = 0; page < 4; page++)
	{
		for (column = 0; column < 128; column++) {
			screenArray[column][page] = 0;
		}
	}
	display_update();
}

/* Clear a single pixel on the specified coordinate (x,y) */
void display_clear_pixel(int x1, int y1){
	/* Check if valid coordinates */
    if(x1 <= 0 || x1 >= 128 || y1 <= 0 || y1 >= 32)
        return;
	
	/* (1,1) is first row and first column, take 1 from x and y due to zero-indexing*/
	int x = x1 - 1;
	int y = y1 - 1;

	/* Setting the byte in the correct row page-wise is aided by the remainder of y % 8 */
	/* Example: y = 18 is the second row in page 3. (18 - 1) % 8 = 1. We need to shift the enable bit 1 spot to the left. */
	int bit_offset = y % 8;                 

	/* Get page number */
    int page  = y / 8; 

    /* Enable the correct bit in the array */
    screenArray[x][page] &= 0x0 << bit_offset;
	display_update();
}

/* Clears all set pixels on the specified page */
void display_clear_page(int page1) {
	/* Check if valid number */
	if (page1 > 4 || page1 < 0)
	{
		return;
	}
	
	int page = page1 - 1;
	int column = 0;

	for (column = 0; column < 128; column++) {
		screenArray[column][page] = 0;
	}
	display_update();
}

/**
 * clear_scoreboard
 * Clears score_board in gamelogic.c
*/
void clear_scoreboard(void) {	
	int page = 0;
	int column;

	for (column = 58; column < 84; column++) {
		screenArray[column][page] = 0;
	}
	display_update();
}

/**
 * display_line
 * Displays a line of pixels from x1 to z1, on line y1
*/
void display_line(int x1,int z1, int y1){
	
	for (x1; x1 < z1 ; x1++)
	{
		display_pixel(x1,y1);
	}

	display_update();
}

/**
 * display_vertical
 * Displays a vertical line from y1 to z1, on row x1
*/
void display_vertical(int y1,int z1, int x1){

	for (y1; y1 < z1; y1++)
	{
		display_pixel(x1,y1);
	}
	display_update();
}

/**
 * Checks if a pixel on position (x1, y1) is lit or not. 
 * Returns 1 if lit, 0 if not lit. 
*/
int check_if_pixel_is_lit(int x1, int y1) {
    // Check if valid coordinates
    if (x1 <= 0 || x1 >= 128 || y1 <= 0 || y1 >= 32)
        return 0; // Consider pixel not lit if coordinates are invalid

    // Adjust coordinates due to zero-indexing
    int x = x1 - 1;
    int y = y1 - 1;

    // Calculate page and bit offset
    int bit_offset = y % 8;
    int page = y / 8;

    // Check if the corresponding bit is set in the screenArray
    return (screenArray[x][page] >> bit_offset) & 0x1;
}


/* quicksleep:  A simple function to create a small delay. Very inefficient use of computing resources,
   but very handy in some special cases.
   Written by Fredrik Lundvall & Axel Isaksson */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

/**
 *  spi_send_recv
 *  Sending and receiving to SPI (?)
 * Written by Fredrik Lundeval & Axel Isaksson
*/
uint8_t spi_send_recv(uint8_t data)
{
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 1));
    return SPI2BUF; // make void?
}

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/**
 * Converts a number to hexadecimal ASCII digits
 * Written by Fredrik Lundeval & Axel Isaksson
*/
void num32asc( char * s, int n ) 
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}		

/* Countdown, called from gamelogic.c */
void count_down() {
    display_clear();
    display_string(62, 14, "3");
    delay(400);
    display_clear();
    display_string(62, 14, "2");
    delay(400);
    display_clear();
    display_string(62, 14, "1");
    delay(400);
    display_clear();
    display_string(58, 14, "GO!");
    delay(400);
    display_clear();
}
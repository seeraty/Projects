# PONG minigame on the ChipKIT Uno32
Authors: Seerat Yasir, Chloe Joyce Wabingga Raneses
This project is part of the course IS1500 Computer Organization and Components at KTH Royal Institute of Technology.

## Requirements
The minigame can be compiled and run on the chipKIT Uno32 development board, or the near identical chipKIT UC32, with a ChipKIT BASIC I/O Shield. Along with these devices, the MCB32 toolchain is also required: https://github.com/is1200-example-projects/mcb32tools/releases/

## Installation
1. After successfully installing the MCB32 toolchain, enter the cross compiler and navigate to the folder with the source code.
2. Compile the code with the `make` command.
3. Install the game into the chipKIT through `make install TTYDEV=/dev/ttySX`, replacing the X with the COM number for your device.
- The COM number can be found in Device manager, under "Ports (COM & LPT)".

## Game instructions
The project makes use of the butons to navigate the menu and play the game, as well as one of the switches for a specific function.

**BTN1** Confirm a selection. Racket down for player 2.

**BTN2** Racket up for player 2.

**BTN3** Racket down for player 1. Scroll through letters in setting an initial menu.

**BTN4** Scroll between options in main menu. Scroll through letters in setting an initial menu. Racket up for player 1.

**SW1** Pause the game.

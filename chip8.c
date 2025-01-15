#include <stdio.h>

#include <stdio.h>
#include <stdio.h>
#include "chip8.h"

chip8 myChip8;

int main(int argc, char **argv)
{

    //Set up render system and register input callbacks
    setupGraphics();
    setupInput();

    //Initialize the Chip8 system and load the game into the memory
    myChip8.initialize();
    myChip8.loadgame("pong");

    //Emulation loop
    for (;;)
    {
        myChip8.emulateCycle();

        if(myChip8.drawFlag){
            drawGraphics();
        }

        myChip8.setKeys();
    }

    return 0;

}

//current opcode
unsigned short opcode;

//4k RAM
unsigned char memory[4096];

//registers
unsigned char V[16];

//index register & program counter
unsigned short I;
unsigned short pc;

//screen
unsigned char gfx[64 * 32];

//timers at 60hz
unsigned char delay_timer;
unsigned char sound_timer;

//stack and stack pointer
unsigned char stack[16];
unsigned char sp;

//HEX based key pad
unsigned char key[16];


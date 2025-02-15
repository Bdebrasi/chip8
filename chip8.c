#include <stdio.h>

#include <stdio.h>
#include <stdio.h>
#include "chip8.h"

chip8 myChip8;

unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};



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


//opcodes 
void (*Chip8Table[17])();
void (*Chip8Arithmetic[16])();

void cpuNULL() 
{
	// Do Nothing
}

//find in chip8Table
Chip8Table[(opcode&0xF000)>>12]();

void cpuARITHMETIC(){
	Chip8Arithmetic[(opcode&0x000F)]();
}


void (*Chip8Table[17]) = 
{
	cpuNULL      , cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, 
	cpuARITHMETIC, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,
	cpuNULL
};

void (*Chip8Arithmetic[16]) = 
{
	cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,
	cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL
};

//The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
void _2NNN(){
    sp++;
    stack[sp] = pc;
    pc = (opcode & 0x0FFF);
}

//Clear the display.
void _00E0(){
    for(int row = 0; row < gfx.length; row++){
        for(int col = 0; col < gfx[row].length; col++){
            gfx[row][col] = 0;           
        }
    }
}

//The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
void _00EE(){
    pc = sp;
    sp--;
}

void(*_00EEfp)();

void (*_2NNNfp)();

void(*_00E0fp)();

_00EEfp = &_00EE;

_2NNNfp = &_2NNN;

_00E0fp = &_00E0;

chip8Table[0] = _2NNNfp;


void chip8::initialize()
{

    //Initalize registers and memory once

    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    //Clear display

    //Clear stack

    //Clear registers V0-VF

    //Clear memory

    //Load fontset
    for(int i=0; i<80; ++i){
        memory[i] = chip8_fontset[i];
    }

    //Reset timers


}

void chip8::emulateCycle()
{
    //Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    //Decode Opcode
    //Execute Opcode


    pc+=2;
    //Update timers
    if(delay_timer > 0)
    --delay_timer;
    if(sound_timer > 0)
    {
        if(sound_timer == 1)
            printf("BEEP!\n");
            --sound_timer;
    }  
}

for(int i = 0; i < bufferSize; ++i)
  memory[i + 512] = buffer[i];



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

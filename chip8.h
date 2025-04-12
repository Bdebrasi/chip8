#ifndef CHIP8_H
#define CHIP8_H

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


//opcode function pointer arrays
void (*Chip8Table[17])();
void (*Chip8Arithmetic[16])();

#endif
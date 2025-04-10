#include <time.h>
#include <stdlib.h>

srand(time(NULL));

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



//To do - make seperate functions to extract bits from opcode. Will be easier to test/debug.

int getFirstHexDigit(int op){
    return op & 0xF000;
}

int getX(int op){
    return op & 0x0F00;
}

int getY(int op){
    return op & 0x00F0;
}

int getNN(int op){
    return op & 0x00FF;
}

int getLastHexDigit(int op){
    return op & 0x000F;
}

int getNNN(int op){
    return op & 0x0FFF;
}

//Clear the display.
void _00E0(){
    int length = 64 * 32;
    for (int i = 0; i < length; i++){
        gfx[i] = 0;
    }
}

//The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
void _00EE(){
    pc = sp;
    sp--;
}

//The interpreter sets the program counter to nnn.
void _1NNN(){
    pc = (opcode & 0x0FFF);
}

//The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
void _2NNN(){
    sp++;
    stack[sp] = pc;
    pc = (opcode & 0x0FFF);
}

//The interpreter compares register Vx to NN, and if they are equal, increments the program counter by 2.
void _3xNN(){
    int NN = getNN(opcode);
    int x = getX(opcode);
    if(NN == V[x]){
        pc+=2;
    }
}

//Skip next instruction if Vx != NN.
void _4xNN(){
    int NN = getNN(opcode);
    int x = getX(opcode);
    if(NN != V[x]){
        pc+=2;
    }
}

//Skip next instruction if Vx = Vy.
void _5xy0(){
    int x = getX(opcode);
    int y = getY(opcode)
    if(V[x] == V[y]){
        pc+=2;
    }
}

//Set Vx = NN. 
void _6xNN(){
    int NN = getNN(opcode);
    int x = getX(opcode);
    V[x] = NN;

}


//Set Vx = Vx + NN.
void _7xNN(){
    int NN = getNN(opcode);
    int x = getX(opcode);
    V[x] += NN;
}

//Set Vx = Vy.
void _8xy0(){
    int x = getX(opcode);
    int y = getY(opcode);
    V[x] = V[y]
}

//Set Vx = Vx OR Vy.
void _8xy1(){
    int x = getX(opcode);
    int y = getY(opcode);
    V[x] = V[x] | V[y]
}

//Set Vx = Vx AND Vy.
void _8xy2(){
    int x = getX(opcode);
    int y = getY(opcode);
    V[x] = V[x] & V[y];
}

//Set Vx = Vx XOR Vy.
void _8xy3(){
    int x = getX(opcode);
    int y = getY(opcode);
    V[x] = V[x] ^ V[y];
}


//Set Vx = Vx + Vy, set VF = carry.
void _8xy4(){
    int x = getX(opcode);
    int y = getY(opcode);
    int sum = V[x] + V[y];
    if (sum > 255){
        V[0x000F] = 1
        sum &= 0x00FF;
    }
    V[x] = sum;
}

//Set Vx = Vx - Vy, set VF = NOT borrow.
void _8xy5(){
    int x = getX(opcode);
    int y = getY(opcode);
    if(V[x] > V[y]){
        V[0x000F] = 1;
    }
    V[x] -= V[y];
}

//Set Vx = Vx SHR 1. V[F] = last bit pre shr.
void _8xy6(){
    int x = getX(opcode);
    V[0x000F] = V[x] & 1;
    V[x] = V[x] >> 1;
}

//8xy7 - SUBN Vx, Vy
//Set Vx = Vy - Vx, set VF = NOT borrow.
void _8xy7(){
    int x = getX(opcode);
    int y = getY(opcode);
    if(V[y] > V[x]){
        V[0x000F] = 1;
    }
    V[x] = V[y] - V[x]
}

//Set Vx = Vx SHL 1.
void _8xyE(){
    int x = getX(opcode);
    int val = V[x];
    if(val & 0x1000){
        V[0x000F] = 1;
    }
    V[x] = val << 1;
}

//Skip next instruction if Vx != Vy.
void _9xy0(){
    int x = getX(opcode);
    int y = getY(opcode);
    if (V[x] != V[y]){
        pc+=2
    }
}

//Set I = nnn.
void _ANNN(){
    I = getNNN(opcode); 
}

//Jump to location nnn + V0.
void _BNNN(){
    int NNN = getNNN(opcode);
    pc = NNN + V[0];
}

//Set Vx = random byte AND kk. T
void _CxNN(){
    int NN = getNN(opcode);
    int randomNumber = rand() % 255;
    int x = getX(opcode);
    V[x] = randomNumber & NN
}

//Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. 
void _DxyN(){
    int N = getN(opcode);
    int x = getX(opcode);
    int y = getY(opcode);
    int pixel;

    for (int colOffset = 0; col < N; colOffset++){
        pixel = memory[I+colOffset];
        for (int rowOffset = 0; row < 8; rowOffset++){
            if (pixel & (0x80>>rowOffset) == 1){
                if(gfx[(x + rowOffset + ((y + colOffset) * 64))] == 1){
                    V[0xF] = 1;
                }
                gfx[x + rowOffset + ((y + colOffset) * 64)] ^= 1;
            }
        }
    }

    drawFlag = True;
    pc+=2;

}


//figure out how to get key later
void _Ex9E(){
    int x = getX(opcode);
    if (key == V[x]){
        pc+=2;
    }
}

//figure out how to get key later
void _ExA1(){
    int x = getX(opcode);
    if (key != V[x]){
        pc+=2;
    }
}

//Do the other V[x] opcodes only store the first few bits? Do I need to explicitly set V[F] = 0 if no carry?
//Don't have access to V registers, pc, gfx etc. since this is now different file. Fix later.

void(*_00E0fp)();
_00E0fp = &_00E0;

void(*_00EEfp)();
_00EEfp = &_00EE;

void(*_1NNNfp)();
_1NNNfp = &_1NNN;

void (*_2NNNfp)();
_2NNNfp = &_2NNN;

void (*_3xNNfp)();
_3xNNfp = &_3xNN;

void (*_4xNNfp)();
_4xNNfp = &_4xNN;

void (*_5xy0fp)();
_5xy0fp = &_5xy0;

void (*_6xNNfp)();
_6xNNfp = &_6xNN;

void (*_7xNNfp)();
_7xNNfp= &_7xNN;

void (*_8xy0fp)();
_8xy0fp= &_8xy0;

void (*_8xy1fp)();
_8xy1fp = &_8xy1;

void (*_8xy2fp)();
_8xy2fp  = &_8xy2;

void (*_8xy3fp)();
_8xy3fp = &_8xy3;

void (*_8xy4fp)();
_8xy4fp = &_8xy4;

void (*_8xy5fp)();
_8xy5fp = &_8xy5;

void (*_8xy6fp)();
_8xy6fp = &_8xy6;

void (*_8xy7fp)();
_8xy7fp = &_8xy7;

void (*_8xyEfp)();
_8xyEfp = &_8xyE;

void (*_9xy0fp)();
_9xy0fp = &_9xy0;

void (*_ANNNfp)();
_ANNNfp = &_ANNN;

void(*_BNNNfp)();
_BNNNfp = &_BNNN;

void(*_CxNNfp)();
_CxNNfp = &_CxNN;

void(*_DxyNfp)();
_DxyNfp = &_DxyN;

void(*_Ex9Efp)();
_Ex9Efp = &_Ex9E;

vodi(*_ExA1fp)();
_ExA1fp = &_ExA1;

chip8Table[0] = _2NNNfp;
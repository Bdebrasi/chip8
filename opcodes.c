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

//Do the other V[x] opcodes only store the first few bits? Do I need to explicitly set V[F] = 0 if no carry?

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

chip8Table[0] = _2NNNfp;
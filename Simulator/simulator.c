/* LC-2K Instruction-level simulator */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;
void printState(stateType *);
int simulateInstruction(stateType *);
int instructionCount;
int opcode,arg0, arg1, arg2;
void getInstruction(int instruction);
int convertNum(int num);


int main(int argc, char *argv[])
{ char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }
    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
         state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    for(int i = 0; i < 8; i++)
        state.reg[i] = 0;
    instructionCount = 1;

    do{
        printState(&state);
    } while (simulateInstruction(&state));

    return(0);
}
void printState(stateType *statePtr)
{ int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}
int simulateInstruction(stateType* currentState){
    if (currentState->pc > currentState->numMemory || currentState->pc < 0){
        printf("error : invalid pc (pc : %d)",currentState->pc);
        exit(1);
    }
    getInstruction(currentState->mem[currentState->pc]);
    switch (opcode) {
        case 0:
            currentState->reg[arg2] = currentState->reg[arg0] + currentState->reg[arg1];
            break;
        case 1:
            currentState->reg[arg2] = ~(currentState->reg[arg0] | currentState->reg[arg1]);
            break;
        case 2:
            arg2 = convertNum(arg2);
            if(currentState->reg[arg0] + arg2 < 0 || currentState->reg[arg0] + arg2 > NUMMEMORY) {
                printf("error : invalid memory access (memory address %d)",currentState->reg[arg0] + arg2);
                exit(1);
            }
            currentState->reg[arg1] = currentState->mem[currentState->reg[arg0] + arg2];
            break;
        case 3:
            arg2 = convertNum(arg2);
            if(currentState->reg[arg0] + arg2 < 0 || currentState->reg[arg0] + arg2 > NUMMEMORY) {
                printf("error : invalid memory access (memory address %d)",currentState->reg[arg0] + arg2);
                exit(1);
            }
            currentState->mem[currentState->reg[arg0] + arg2] = currentState->reg[arg1];
            break;
        case 4:
            arg2 = convertNum(arg2);
            if(currentState->reg[arg0] == currentState->reg[arg1])
                currentState->pc += (arg2); //pc + 1 + offsetField
            break;
        case 5:
            currentState->reg[arg1] = currentState->pc + 1;
            currentState->pc = currentState->reg[arg0] - 1; // pc = regA
            break;
        case 6:
            currentState->pc++;
            printf("machine halted\n");
            printf("total of %d instructions executed\n", instructionCount);
            printf("final state of machine:");
            printState(currentState);
            return 0;
        case 7:
            break;
        default:
            printf("error : invalid opcode (opcode : %#x)\n", opcode);
            exit(1);
    }
    instructionCount++;
    currentState->pc++;
    return 1;
}

void getInstruction(int instruction){
    opcode = instruction & (0b1111111111 << 22);
    opcode >>= 22;
    arg0 = instruction & (0b111 << 19);
    arg0 >>= 19;
    arg1 = instruction & (0b111 << 16);
    arg1 >>= 16;
    arg2 = instruction & (0b1111111111111111);
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}
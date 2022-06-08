/* Assembler code fragment for LC-2K */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINELENGTH 1000

typedef struct LinkedList* LabelList;

struct LinkedList{
    char name[7];
    int address;
    LabelList next;
};
LabelList create();
void insert(LabelList labels, char* k, int v);
int find(LabelList labels, char* k);
int addressCount;
int convertNum(int num);

LabelList create(){
    LabelList new = (LabelList) malloc(sizeof (struct LinkedList));
    new->name[0] = '\0';
    new->next = NULL;
    return new;
}
void insert(LabelList labels,char *k,int v){
    if(!labels->name[0]){
        strcpy(labels->name,k);
        labels->address = v;
        return;
    }
    LabelList current;
    for(current = labels; current->next; current = current->next)
        ;
    LabelList new = (LabelList) malloc(sizeof (struct LinkedList));
    strcpy(new->name, k);
    new->address = v;
    new->next = NULL;
    current->next = new;
}

int find(LabelList labels, char *k){
    LabelList current;
    for(current = labels; current; current = current->next){
        if (!strcmp(current->name, k))
            return current->address;
    }
    return -1;
}

int convertNum(int num){
    if (num < -32768){
        printf("error : invalid offset field (%d)\n",num);
        printf("error line : %d", addressCount);
        return 1;
;    }
    if (num > 32767){
        printf("error : invalid offset field (%d)\n",num);
        printf("error line : %d", addressCount);
        return 1;
    }
    if (num < 0){
        num = num & (0b1111111111111111);
    }
    return num;
}



int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int main(int argc, char *argv[]) {
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
               argv[0]);
        exit(1);
    }
    inFileString = argv[1];
    outFileString = argv[2];
    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* here is an example for how to use readAndParse to read a line from
    inFilePtr */
    LabelList labelDictionary = create();
    addressCount = 0;
    while (1) {
        if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
            /* reached end of file */
            break;
        }
        if(!label[0]){
            addressCount++;
            continue;
        }
        else if (strlen(label) > 6 || !isalpha(label[0])) {
            printf("error: invalid label name: %s\n", label);
            printf("error line : %d\n",addressCount); return 1;
        }
        else if (strlen(label) > 0){
            if (find(labelDictionary,label) != -1){
                printf("error: duplicate label name (%s)\n", label);
                printf("error line : %d\n",addressCount); return 1;
            }
            insert(labelDictionary, label, addressCount);
        }
        addressCount++;
    }
    /* this is how to rewind the file ptr so that you start reading from the
    beginning of the file */
    rewind(inFilePtr);
    /* after doing a readAndParse, you may want to do the following to test the
    opcode */
    int instruction, resA, resB, destRegOrOffset;
    addressCount = 0;
    while (1) {
        if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
            /* reached end of file */
            break;
        }
        instruction = 0;
        if (!strcmp(opcode, "add")) {
            /* do whatever you need to do for opcode "add" */
            if (isNumber(arg0)) {
                sscanf(arg0, "%d", &resA);

                resA = resA << 19;
                instruction += resA;
            }
            if (isNumber(arg1)) {
                sscanf(arg1, "%d", &resB);
                if(resB < 0 || resB >= 8){
                    printf("error : invalid register number (resB : %d)\n",resB);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resB = resB << 16;
                instruction += resB;
            }
            if(isNumber(arg2)){
                sscanf(arg2,"%d",&destRegOrOffset);
                if(destRegOrOffset <= 0 || destRegOrOffset >= 8){
                    printf("error : invalid register number (destRes : %d)\n",destRegOrOffset);
                    printf("error line : %d\n",addressCount); return 1;
                }
                instruction += destRegOrOffset;
            }
        }
        else if (!strcmp(opcode, "nor")) {
            /* do whatever you need to do for opcode "add" */
            if (isNumber(arg0)) {
                sscanf(arg0, "%d", &resA);
                if(resA < 0 || resA >= 8){
                    printf("error : invalid register number (resA : %d)\n",resA);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resA = resA << 19;
                instruction += resA;
            }
            if (isNumber(arg1)) {
                sscanf(arg1, "%d", &resB);
                if(resB < 0 || resB >= 8){
                    printf("error : invalid register number (resB : %d)\n",resB);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resB = resB << 16;
                instruction += resB;
            }
            if(isNumber(arg2)){

                sscanf(arg2,"%d",&destRegOrOffset);
                if(destRegOrOffset <= 0 || destRegOrOffset >= 8){
                    printf("error : invalid register number (destRes : %d)\n",destRegOrOffset);
                    printf("error line : %d\n",addressCount); return 1;
                }
                instruction += destRegOrOffset;
            }
            instruction += 1 << 22;
        }
        else if (!strcmp(opcode, "lw")) {
            /* do whatever you need to do for opcode "add" */
            if (isNumber(arg0)) {
                sscanf(arg0, "%d", &resA);
                if(resA < 0 || resA >= 8){
                    printf("error : invalid register number (resA : %d)\n",resA);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resA = resA << 19;
                instruction += resA;
            }
            if (isNumber(arg1)) {
                sscanf(arg1, "%d", &resB);
                if(resB <= 0 || resB >= 8){
                    printf("error : invalid register number (resB : %d)\n",resB);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resB = resB << 16;
                instruction += resB;
            }
            if(isNumber(arg2))
                sscanf(arg2,"%d",&destRegOrOffset);
            else if (find(labelDictionary,arg2) != -1)
                destRegOrOffset = find(labelDictionary,arg2);
            else {
                printf("invalid label\n");
                printf("error line : %d\n",addressCount); return 1;
            }
            instruction += convertNum(destRegOrOffset);
            instruction += 2 << 22;
        }
        else if (!strcmp(opcode, "sw")) {
            /* do whatever you need to do for opcode "add" */
            if (isNumber(arg0)) {
                sscanf(arg0, "%d", &resA);
                if(resA < 0 || resA >= 8){
                    printf("error : invalid register number (resA : %d)\n",resA);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resA = resA << 19;
                instruction += resA;
            }
            if (isNumber(arg1)) {
                sscanf(arg1, "%d", &resB);
                if(resB < 0 || resB >= 8){
                    printf("error : invalid register number (resB : %d)\n",resB);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resB = resB << 16;
                instruction += resB;
            }
            if(isNumber(arg2))
                sscanf(arg2,"%d",&destRegOrOffset);
            else if (find(labelDictionary,arg2) != -1)
                destRegOrOffset = find(labelDictionary,arg2);
            else {
                printf("invalid label\n");
                printf("error line : %d\n",addressCount); return 1;
            }
            instruction += convertNum(destRegOrOffset);
            instruction += 3 << 22;
        }
        else if (!strcmp(opcode, "beq")) {
            /* do whatever you need to do for opcode "add" */
            if (isNumber(arg0)) {
                sscanf(arg0, "%d", &resA);
                if(resA < 0 || resA >= 8){
                    printf("error : invalid register number (resA : %d)\n",resA);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resA = resA << 19;
                instruction += resA;
            }
            if (isNumber(arg1)) {
                sscanf(arg1, "%d", &resB);
                if(resB < 0 || resB >= 8){
                    printf("error : invalid register number (resB : %d)\n",resB);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resB = resB << 16;
                instruction += resB;
            }
            if(isNumber(arg2))
                sscanf(arg2,"%d",&destRegOrOffset);
            else if (find(labelDictionary,arg2) != -1)
                destRegOrOffset = find(labelDictionary,arg2) - addressCount -1;
            else {
                printf("invalid label\n");
                printf("error line : %d\n",addressCount); return 1;
            }
            instruction += convertNum(destRegOrOffset);
            instruction += 4 << 22;
        }
        else if (!strcmp(opcode, "jalr")) {
            /* do whatever you need to do for opcode "add" */
            if (isNumber(arg0)) {
                sscanf(arg0, "%d", &resA);
                if(resA < 0 || resA >= 8){
                    printf("error : invalid register number (resA : %d)\n",resA);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resA = resA << 19;
                instruction += resA;
            }
            if (isNumber(arg1)) {
                sscanf(arg1, "%d", &resB);
                if(resB < 0 || resB >= 8){
                    printf("error : invalid register number (resB : %d)\n",resB);
                    printf("error line : %d\n",addressCount); return 1;
                }
                resB = resB << 16;
                instruction += resB;
            }
            instruction += 5 << 22;
        }
        else if (!strcmp(opcode,"halt")){
            instruction += 6 << 22;
        }
        else if (!strcmp(opcode,"noop")){
            instruction += 7 << 22;
        }
        else if (!strcmp(opcode,".fill")){
            if(isNumber(arg0))
                sscanf(arg0, "%d", &resA);
            else if(find(labelDictionary,arg0) != -1)
                resA = find(labelDictionary,arg0);
            else {
                printf("error: invalid label\n");
                printf("%s",arg0);
                printf("error line : %d\n",addressCount); return 1;
            }
            instruction += resA;
        }
        else {
            printf("error: unrecognized opcode\n" );
            printf("%s", opcode);
            printf("error line : %d\n",addressCount); return 1;
        }
        fprintf(outFilePtr,"%d\n",instruction);
        addressCount++;
    }
    return(0);
}
/*
* Read and parse a line of the assembly-language file. Fields are returned
* in label, opcode, arg0, arg1, arg2 (these strings must have memory already
* allocated to them). *
* Return values:
* 0 if reached end of file
* 1 if all went well *
* exit(1) if line is too long.
*/
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
                 char *arg1, char *arg2)
{ char line[MAXLINELENGTH];
    char *ptr = line;
    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';
    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }
    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }
    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }
    /*
    * Parse the rest of the line. Would be nice to have real regular
    * expressions, but scanf will suffice.
    */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
    return(1);
}
int isNumber(char *string) { /* return 1 if string is a number */
    int i;
    return( sscanf(string, "%d", &i) == 1);
}

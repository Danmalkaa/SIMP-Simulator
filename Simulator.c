//
//  Simulator.c
//  SIMP Assembler
//
//  Created by Dan Malka on 03/01/2021.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 256
#define MAXLABELSIZE 10
#define MAXSIZE 80

void add(int* rd, int* rs, int* rt)
{
    *rd = *rs + *rt;
}
void sub(int* rd, int* rs, int* rt)
{
    *rd = *rs - *rt;
}
void and(int* rd, int* rs, int* rt)
{
    *rd = *rs & *rt;
}
void or(int* rd, int* rs, int* rt)
{
    *rd = *rs | *rt;
}
void xor(int* rd, int* rs, int* rt)
{
    *rd = *rs ^ *rt;
}
void mul(int* rd, int* rs, int* rt)
{
    *rd = *rs * *rt;
}
void sll(int* rd, int* rs, int* rt)
{
    *rd = *rs << *rt;
}
void sra(int* rd, int* rs, int* rt) //check if correct- shift with sign extension
{
    *rd = *rs >> *rt;
}
void srl(int* rd, int* rs, int* rt)//logical shift- check if correct
{
    *rd = *rs >> *rt;
}

/* an auxilary struct to be used as a for functions */
typedef void (*funct)(int* rd, int* rs, int* rt);

typedef struct func_struct {
    funct function;
}func_struct;

/* an auxilary struct to be used as a dictionary */
typedef struct  { func_struct func; char* str; }func_dict;
func_dict opcode_to_func_dict [] =
{
    { add, "00"},
    { sub, "01"},
    { and, "02"},
    { or, "03"},
    { xor, "04" },
    { mul, "05" },
    { sll, "06" },
    { sra, "07" },
    { srl, "08" },
//    { "beq", "09" },
//    { "bne", "0A" },
//    { "blt", "0B" },
//    { "bgt", "0C" },
//    { "ble", "0D" },
//    { "bge", "0E"},
//    { "jal", "0F" },
//    { "lw", "10" },
//    { "sw", "11" },
//    { "reti", "12" },
//    { "in", "13" },
//    { "out", "14" },
//    { "halt", "15" },
};
func_struct* getFunct(char * str, int mode)
{
    func_dict* pSearch;
    func_struct* selected = NULL;
    if (str != NULL)
    {
        switch (mode)
        {
            case 1: // command search
                /* runing on the dictionary to get the opcode selected */
                for(pSearch = opcode_to_func_dict; pSearch != opcode_to_func_dict + sizeof(opcode_to_func_dict) / sizeof(opcode_to_func_dict[0]); pSearch++ )
                {
                    if(!strcmp( pSearch->str, str))
                        selected = &(pSearch->func);
                }
                if (selected==NULL)
                    printf("%s is not a valid command name\n", str);
                break;
        }
    }
    else
        printf("expected opcocde name, got NULL\n");
    return selected;
}

///* an auxilary struct to be used as a dictionary */
typedef struct  { int* reg; char* hexa; }reg_dict;
int $zero=0,$imm=0,$v0=0,$a0=0,$a1=0,$t0=0,$t1=0,$t2=0,$t3=0;
int $s0=0,$s1=0,$s2=0,$gp=0,$sp=0,$fp=0,$ra=0;
reg_dict register_from_hexa_dict [] =
{
    { &$zero, "0"},
    { &$imm, "1"},
    { &$v0, "2"},
    { &$a0, "3"},
    { &$a1, "4" },
    { &$t0, "5" },
    { &$t1, "6" },
    { &$t2, "7" },
    { &$t3, "8" },
    { &$s0, "9" },
    { &$s1, "A" },
    { &$s2, "B" },
    { &$gp, "C" },
    { &$sp, "D" },
    { &$fp, "E"},
    { &$ra, "F" },
};

int* getReg(char * str, int mode)
{
    reg_dict* rSearch;
    int* selected = NULL;
    if (str != NULL)
    {
        switch (mode)
        {
            case 1: // command search
                /* runing on the dictionary to get the register selected */
                for(rSearch = register_from_hexa_dict; rSearch != register_from_hexa_dict + sizeof(register_from_hexa_dict) / sizeof(register_from_hexa_dict[0]); rSearch++ )
                {
                    if(!strcmp( rSearch->hexa, str))
                        selected = rSearch->reg;
                }
                if (selected==NULL)
                    printf("%s is not a valid command name\n", str);
                break;
        }
    }
    else
        printf("expected opcocde name, got NULL\n");
    return selected;
}
int main(void)
{
    int *rd=NULL, rs=10, rt=5, *j;
    int var = 20;
    rd = &var;
    func_struct *test;
    test = getFunct("00", 1);
    test->function(rd,&rs,&rt);
    j = getReg("0", 1);
    test->function(j,&rs,&rt);
    return 0;
}

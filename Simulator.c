#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 256
#define MAXLABELSIZE 50
#define MAXSIZE 500

void add(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = *rs + *rt;
}
void sub(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = *rs - *rt;
}
void and(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = *rs & *rt;
}
void or (int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = *rs | *rt;
}
void xor(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = *rs ^ *rt;
}
void mul(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = *rs * *rt;
}
void sll(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = *rs << *rt;
}
void sra(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io) //check if correct- shift with sign extension
{
    *rd = *rs >> *rt;
}
void srl(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)//logical shift- check if correct
{
    *rd = *rs >> *rt;
}
void beq(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    if (*rs == *rt)
        *pc = *rd;
}
void bne(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    if (*rs != *rt)
        *pc = *rd;
}
void blt(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    if (*rs < *rt)
        *pc = *rd;
}
void bgt(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    if (*rs > *rt)
        *pc = *rd;
}
void ble(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    if (*rs <= *rt)
        *pc = *rd;
}
void bge(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    if (*rs >= *rt)
        *pc = *rd;
}
void jal(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *ra_io = *pc;
    *pc = *rd;
}
void reti(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *pc = *io_reg;
}
void lw(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = datamemmory[*rs + *rt];
}
void sw(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    datamemmory[*rs + *rt]= *rd;
}
void in(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *rd = *io_reg;
}
void out(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    *io_reg = *rd;
}
void halt(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
{
    return;
}


/* auxilary structs to be used for functions */
typedef void(*funct)(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io);
typedef void(*funct_2)(int* rd, int* rs, int* rt, int* pc);
typedef void(*funct_3)(int* rd, int* pc, int* ra_io);
typedef void(*funct_4)(int* rd, int datamemmory[4096]);
typedef void(*funct_5)(int* rd, int* io_reg);


typedef struct func_struct {
    funct function;
    int func_type;
}func_struct;



/* an auxilary struct to be used as a dictionary */
typedef struct { func_struct func; char* str; }func_dict;
func_dict opcode_to_func_dict[] =
{
    { add,1, "00"},
    { sub,1, "01"},
    { and,1, "02"},
    { or ,1, "03"},
    { xor,1, "04" },
    { mul,1, "05" },
    { sll,1, "06" },
    { sra,1, "07" },
    { srl,1, "08" },
    { beq,2, "09" },
    { bne,2, "0A" },
    { blt,2, "0B" },
    { bgt,2, "0C" },
    { ble,2, "0D" },
    { bge,2, "0E"},
    { jal,3, "0F" },
    { lw,4, "10" },
    { sw,4, "11" },
    { reti,3, "12" },
    { in,5, "13" },
    { out,5, "14" },
    { halt,6, "15" },
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
            for (pSearch = opcode_to_func_dict; pSearch != opcode_to_func_dict + sizeof(opcode_to_func_dict) / sizeof(opcode_to_func_dict[0]); pSearch++)
            {
                if (!strcmp(pSearch->str, str))
                    selected = &(pSearch->func);
            }
            if (selected == NULL)
                printf("%s is not a valid command name\n", str);
            break;
        }
    }
    else
        printf("expected opcocde name, got NULL\n");
    return selected;
}

///* an auxilary struct to be used as a dictionary */
typedef struct  { int* reg; char hexa; }reg_dict;
int* getReg(char str, int mode, reg_dict register_from_hexa_dict[16], int one_size)
{
    reg_dict rSearch;
    int* selected = NULL;
    if (str != '\0')
    {
        switch (mode)
        {
            case 1: // command search
                /* runing on the dictionary to get the register selected */
                for(int i = 0; i <16 ; i++ )
                {
                    rSearch =register_from_hexa_dict[i];
                    if (rSearch.hexa == str){
                        selected = rSearch.reg;
                        break;
                    }
                }
                if (selected==NULL)
                    printf("%c is not a valid command name\n", str);
                break;
        }
    }
    else
        printf("expected opcocde name, got NULL\n");
    return selected;
}

int readdmemin(FILE* dmemin, int datamemmory[4096]) { // copy dmemin to array
    char linestr[10];
    int linedata, linenumber = 0;
    while (feof(dmemin)==0)
    {
        fgets(linestr, 10, dmemin);
        linedata = strtol(linestr, NULL, 16);
        datamemmory[linenumber] = linedata;
        linenumber++;
    }
    return 0;
}

int readimemin(FILE* imemin, int imemmory[4096]) { // copy dmemin to array
    char linestr[10];
    int linedata, linenumber = 0;
    while (feof(dmemin)==0)
    {
        fgets(linestr, 10, dmemin);
        linedata = linestr;
        imemmory[linenumber] = linedata;
        linenumber++;
    }
    return 0;
}

int readdiskin(FILE* diskin, int diskmemmory[128][64]) { // copy diskin to matrix
    char linestr[10];
    int linedata, linenumber = 0, sectornumber = 0;
    while (feof(diskin) == 0)
    {
        fgets(linestr, 10, diskin);
        linedata = strtol(linestr, NULL, 16);
        diskmemmory[sectornumber][linenumber] = linedata;
        linenumber++;
        if (linenumber == 63) {
            sectornumber++;
            linenumber = 0;
        }
    }
    return 0;
}



//this function print into trace file
void printTrace(arg[trace], int pc,char inst, int $zero, int $imm, int $v0, int $a0, int $a1, int $t0, int $t1, int $t2, int $t3,int $s0, int $s1, int $s2, int $gp, int $sp, int $fp, int $ra)  {
    int inst_int;
    FILE *trace;

    trace = fopen(arg[trace], "a");
    if (trace == NULL)
        return;
    inst_int = strtol(inst, NULL, 4);
    printf("%03X %05X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n", pc, inst_int, $zero, $imm, $v0, $a0, $a1, $t0, $t1, $t2, $t3, $s0, $s1, $s2, $gp, $sp, $fp, $ra);
    return;
}

int main(void) // *add the arguments for the input*
{
    int $zero=0, $imm=0, $v0=0, $a0=0, $a1=0, $t0=0, $t1=0, $t2=0, $t3=0; //registers_vars
    int $s0=0, $s1=0, $s2=0, $gp=0, $sp=0, $fp=0, $ra=0; //registers_vars
    int irq0enable = 0, irq1enable = 0, irq2enable = 0, irqhandler = 0; //hardware registers
    int irq0status = 0, irq1status = 0, irq2status = 0, irqreturn = 0; //hardware registers
    int clks = 0, leds = 0, reserved = 0, timerenable = 0, timercurrent = 0, timermax = 0; //hardware registers
    int diskcmd = 0, disksector = 0, diskbuffer = 0, diskstatus = 0; //hardware registers
    int monitorcmd = 0, monitorx = 0, monitory = 0, monitordata = 0; //hardware registers
    int io_reg_array[22] = { 0 };

    reg_dict register_from_hexa_dict [] =
    {
    { &$zero, '0'},
    { &$imm, '1'},
    { &$v0, '2'},
    { &$a0, '3'},
    { &$a1, '4' },
    { &$t0, '5' },
    { &$t1, '6' },
    { &$t2, '7' },
    { &$t3, '8' },
    { &$s0, '9' },
    { &$s1, 'A' },
    { &$s2, 'B' },
    { &$gp, 'C' },
    { &$sp, 'D' },
    { &$fp, 'E'},
    { &$ra, 'F' },
    };
    int pc = 0, immon = 0, clock = 0, cmdcounter = 0, irq2next, irq, reti = 0, halt_flag=0; //counters and controllers
    FILE *imemin, *dmemin, *diskin, *irq2in;
    FILE *dmemout, *trace, *regout, *hwregtrace, *cycles;
    FILE *fleds, *monitor, *diskout;
    char irq2line[MAXSIZE];
    int cmdmemmory[1024] = { 0 }, datamemmory[4096] = { 0 }, diskmemmory[128][64] = { 0 }, imemmory[4096] = { 0 }; //initialize arrays for inner memmory and matrix for disk memmory

    imemin = fopen(argv[0], "r"); // read imemin.txt and copy it content into **string** array
    if (dmemin != NULL) {
        readimemin(imemin, imemmory);
        fclose(imemin);
    }
    else
        printf("imemin.txt empty or corrupted");
    
    dmemin = fopen(argv[1], "r"); // read dmemin.txt and copy it content into **int** array
    if (dmemin != NULL) {
        readdmemin(dmemin, datamemmory);
        fclose(dmemin);
    }
    else
        printf("dmemin.txt empty or corrupted");

    diskin = fopen(argv[2], "r"); // read diskin.txt and copy it content into **int** matrix
    if (diskin != NULL) {
        readdiskin(diskin, diskmemmory);
        fclose(diskin);
    }
    else
        printf("diskin.txt empty or corrupted");

    irq2in = fopen(argv[3], "r"); // read irq2in.txt, read and cast the first line into int
    if (irq2in == NULL)
        printf("irq2in.txt empty or corrupted");
    fgets(irq2line, MAXSIZE, irq2in);
    irq2next = strtol(irq2line, NULL, 10);

    irq = (irq0enable & irq0status) | (irq1enable & irq1status) | (irq2enable & irq2status); //irq check
    if (irq == 1 & reti == 0) { //jump to irq handler
        irqreturn = pc;
        pc = irqhandler;
        reti = 1;
    }
    else { //read the cmd
        char buffer[MAX_LEN];
        buffer = imemmory[pc];
        // -1 to allow room for NULL terminator for long string
        int rd_is_zero =0, imm_reg_count = 0, snd_imm_reg_count =0, trd_imm_reg_count =0;
        int immediate = 0;
        // Remove trailing newline
//        buffer[strcspn(buffer, "\n")] = 0; // sets the buffer to 0 after the index for end of line
        char * pch=NULL;
        int k = 0;
        char parameters[5]; // init the 5 commands array in the line
        pch = strtok (buffer," ,\t:");
        while (*pch != '\0')
        {
            if (*pch == '\n') // reached end of line
            {
              *pch = '\0'; // to get out of loop
              continue;
            }
            parameters[k++] = *pch;//assigns the command in the 5 commands 1-line array
            pch++; // gets next char
            if (k>1){ // k=0,1 are for opcode
                if (*pch == '1'){ // saves which register is the immediate one in the current line
                    immon =1;
                    if (imm_reg_count){ // if one is already imm
                        if (snd_imm_reg_count) // if a second one is already imm - snd=second
                            trd_imm_reg_count = k; // trd=third
                        else
                            snd_imm_reg_count =k;
                    }
                    else
                        imm_reg_count = k;
                }
            }
            if (k==2)
            if (*pch == '0')
                rd_is_zero = 1;
        }
        if (immon)
        {
            immediate = 0;
            char* imm_str = NULL;
            buffer = imemmory[pc+1];
            buffer[strcspn(buffer, "\n")] = 0;
            imm_str = strtok (buffer," ,\t:");
            immediate = strtol(imm_str, NULL, 16);
            $imm = immediate;
        }
        int  *rs, *rt, *rd, *io_reg, no_imm_rd_opcode = 0, func_type=0, is_reti = 0, loc = 0;
        char opcode[3];
        opcode[0] = parameters[0];
        opcode[1] = parameters[1];
        opcode[2] = '\0';
        func_struct *op_func;
        op_func = getFunct(opcode, 1);
        func_type = op_func->func_type;
        rd = getReg(parameters[2], 1,register_from_hexa_dict);
        rs = getReg(parameters[3], 1,register_from_hexa_dict);
        rt = getReg(parameters[4], 1,register_from_hexa_dict);
        if (!strcmp(opcode,"12"))
            is_reti = 1;
        switch (func_type)
        {
            case 1: // // the opcode cant assign values to imm or zero
                if (immon){
                    if (rd_is_zero)// not possible to set values to $zero
                        continue;
                    else if (imm_reg_count == 2) // not possible to set values to $imm
                        continue;
                    else{
                        op_func->function(rd,rs,rt,NULL,NULL,NULL,NULL);
                        break;
                    }
                }
                else
                    op_func->function(rd,rs,rt,NULL,NULL,NULL,NULL);
                break;
            case 2: // // the opcode with pc
                op_func->function(rd,rs,rt,NULL,NULL,&pc,NULL);
                break;
            case 3: // // reti or jal
                if (is_reti){
                    io_reg = &io_reg_array[7];
                    op_func->function(NULL,NULL,NULL,NULL,io_reg,&pc,NULL);
                    reti = 1;
                }
                else // jal
                    op_func->function(rd,NULL,NULL,NULL,NULL,&pc,&$ra);


                break;
            case 4: // // the opcode uses dmemmory
                op_func->function(rd,rs,rt,datamemmory,NULL,NULL,NULL);
                break;
            case 5: // // the opcode uses IO registers
                io_reg = &io_reg_array[*rs+*rt];
                op_func->function(rd,NULL,NULL,NULL,io_reg,NULL,NULL);
                break;
            case 6: // // *Halt - need to add what else happens - prints etc.*
                halt_flag=1;
                break;

        }
        
        printf("%d\n",$imm); // debug
        }
        return 0;
    }
        
        
    }
}

//void halt(int* rd, int* rs, int* rt, int datamemmory[4096], int* io_reg, int* pc, int* ra_io)
///* auxilary structs to be used for functions */
//typedef void(*funct_1)(int* rd, int* rs, int* rt);
//typedef void(*funct_2)(int* rd, int* rs, int* rt, int* pc);
//typedef void(*funct_3)(int* rd, int* pc, int* ra_io);
//typedef void(*funct_4)(int* rd, int datamemmory[4096]);
//typedef void(*funct_5)(int* rd, int* io_reg);


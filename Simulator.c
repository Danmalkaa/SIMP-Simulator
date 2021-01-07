#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 256
#define MAXLABELSIZE 50
#define MAXSIZE 500

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
void or (int* rd, int* rs, int* rt)
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
typedef void(*funct)(int* rd, int* rs, int* rt);

typedef struct func_struct {
	funct function;
        int imm_zero;
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
// typedef struct { int* reg; char* hexa; }reg_dict;
// int $zero = 0, $imm = 0, $v0 = 0, $a0 = 0, $a1 = 0, $t0 = 0, $t1 = 0, $t2 = 0, $t3 = 0; //registers
// int $s0 = 0, $s1 = 0, $s2 = 0, $gp = 0, $sp = 0, $fp = 0, $ra = 0; //registers
// int irq0enable = 0, irq1enable = 0, irq2enable = 0, irqhandler = 0; //hardware registers
// int irq0status = 0, irq1status = 0, irq2status = 0, irqreturn = 0; //hardware registers
// int clks = 0, leds = 0, reserved = 0, timerenable = 0, timercurrent = 0, timermax = 0; //hardware registers
// int diskcmd = 0, disksector = 0, diskbuffer = 0, diskstatus = 0; //hardware registers
// int monitorcmd = 0, monitorx = 0, monitory = 0, monitordata = 0; //hardware registers
// reg_dict register_from_hexa_dict[] =
// {
// 	{ &$zero, "0"},
// 	{ &$imm, "1"},
// 	{ &$v0, "2"},
// 	{ &$a0, "3"},
// 	{ &$a1, "4" },
// 	{ &$t0, "5" },
// 	{ &$t1, "6" },
// 	{ &$t2, "7" },
// 	{ &$t3, "8" },
// 	{ &$s0, "9" },
// 	{ &$s1, "A" },
// 	{ &$s2, "B" },
// 	{ &$gp, "C" },
// 	{ &$sp, "D" },
// 	{ &$fp, "E"},
// 	{ &$ra, "F" },
// };

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

int main(void)
{
	int v$zero=0, v$imm=0, v$v0=0, v$a0=0, v$a1=0, v$t0=0, v$t1=0, v$t2=0, v$t3=0; //registers_vars
	int v$s0=0, v$s1=0, v$s2=0, v$gp=0, v$sp=0, v$fp=0, v$ra=0; //registers_vars
	int *$zero =&v$zero, *$imm=&v$imm, *$v0=&v$v0, *$a0=&v$a0, *$a1=&v$a1, *$t0=&v$t0, *$t1=&v$t1, *$t2=&v$t2, *$t3=&v$t3; //registers_pointers
	int *$s0=&v$s0, *$s1=&v$s1, *$s2=&v$s2, *$gp=&v$gp, *$sp=&v$sp, *$fp=&v$fp, *$ra=&v$ra; //registers_pointers
	int irq0enable = 0, irq1enable = 0, irq2enable = 0, irqhandler = 0; //hardware registers
	int irq0status = 0, irq1status = 0, irq2status = 0, irqreturn = 0; //hardware registers
	int clks = 0, leds = 0, reserved = 0, timerenable = 0, timercurrent = 0, timermax = 0; //hardware registers
	int diskcmd = 0, disksector = 0, diskbuffer = 0, diskstatus = 0; //hardware registers
	int monitorcmd = 0, monitorx = 0, monitory = 0, monitordata = 0; //hardware registers
	reg_dict register_from_hexa_dict [] =
	{
	{ $zero, '0'},
	{ $imm, '1'},
	{ $v0, '2'},
	{ $a0, '3'},
	{ $a1, '4' },
	{ $t0, '5' },
	{ $t1, '6' },
	{ $t2, '7' },
	{ $t3, '8' },
	{ $s0, '9' },
	{ $s1, 'A' },
	{ $s2, 'B' },
	{ $gp, 'C' },
	{ $sp, 'D' },
	{ $fp, 'E'},
	{ $ra, 'F' },
	};
	int *rd, *rs, *rt; // command registers
	int pc = 0, immon = 0, clock = 0, cmdcounter = 0, irq2next, irq, reti = 0; //counters and controllers
	FILE *imemin, *dmemin, *diskin, *irq2in; 
	FILE *dmemout, *trace, *regout, *hwregtrace, *cycles;
	FILE *leds, *monitor, *diskout;
	char irq2line[MAXSIZE];
	int cmdmemmory[1024] = { 0 }, datamemmory[4096] = { 0 }, diskmemmory[128][64] = { 0 }; //initialize arrays for inner memmory and matrix for disk memmory


	dmemin = fopen(argv[1], "r"); // read dmemin.txt and copy it content into **int** array 
	if (dmemin != NULL) {
		readdmemin(dmemin, datamemmory);
		fclose(dmemin);
	}
	else
		printf("dmemin.txt empty or corrapted");

	diskin = fopen(argv[2], "r"); // read diskin.txt and copy it content into **int** matrix
	if (diskin != NULL) {
		readdiskin(diskin, diskmemmory);
		fclose(diskin);
	}
	else
		printf("diskin.txt empty or corrapted");

	irq2in = fopen(argv[3], "r"); // read irq2in.txt, read and cast the first line into int
	if (irq2in == NULL)
		printf("irq2in.txt empty or corrapted");
	fgets(irq2line, MAXSIZE, irq2in);
	irq2next = strtol(irq2line, NULL, 10);

	irq = (irq0enable & irq0status) | (irq1enable & irq1status) | (irq2enable & irq2status); //irq check
	if (irq == 1 & reti == 0) { //jump to irq handler
		irqreturn = pc;
		pc = irqhandler;
		reti = 1;
	}
	else { //read the cmd 
	    FILE *imemin; // need to fix this part
	    imemin = fopen(argv[0], "r");
	    if (imemin == NULL) {
	      perror("Failed: ");
	      return 1;
	    }
	    char buffer[MAX_LEN];
	    // -1 to allow room for NULL terminator for long string
	    while (fgets(buffer, MAX_LEN - 1, imemin))
	    {
		int rd_is_zero =0, imm_reg_count = 0, snd_imm_reg_count =0, trd_imm_reg_count =0;
		int immediate = 0;
		// Remove trailing newline
		buffer[strcspn(buffer, "\n")] = 0; // sets the buffer to 0 after the index for end of line
		char * pch=NULL;
		int k = 0;
		char parameters[5]; // init the 5 commands array in the line
		pch = strtok (buffer," ,\t:");
		while (*pch != '\0')
		{
		    if (*pch == '#') // reached comments
		    {
		      pch = NULL; // to get out of loop
		      continue;
		    }
		    parameters[k++] = *pch;//assigns the command in the 5 commands 1-line array
		    pch++; // gets next char
		    if (k>1){
			if (*pch == '1'){ // saves which register is the immediate one in the current line
			    immon =1;
			    if (imm_reg_count)
				if (snd_imm_reg_count)
				    trd_imm_reg_count = k;
				else
				    snd_imm_reg_count =k;
			    else
				imm_reg_count = k;
			}
		    }
		    if (k==2)
			if (*pch == '0')
			    rd_is_zero = 1;
		}
		if (is_imm)
		{
		    immediate = 0;
		    char* imm_str = NULL;
		    fgets(buffer, MAX_LEN - 1, imemin);
		    buffer[strcspn(buffer, "\n")] = 0;
		    imm_str = strtok (buffer," ,\t:");
		    immediate = strtol(imm_str, NULL, 16);
		    v$imm = immediate;
		}
	    int  *rs, *rt, *rd, no_imm_rd_opcode = 0;
	    char opcode[3];
	    opcode[0] = parameters[0];
	    opcode[1] = parameters[1];
	    opcode[2] = '\0';
	    func_struct *op_func;
	    op_func = getFunct(opcode, 1);
		if (op_func->imm_zero)
		    no_imm_rd_opcode = 1;
	//    op_func->function(j,rs,rt);
	    rd = getReg(parameters[2], 1,register_from_hexa_dict);
	    rs = getReg(parameters[3], 1,register_from_hexa_dict);
	    rt = getReg(parameters[4], 1,register_from_hexa_dict);
	    if (immon){
		    if (no_imm_rd_opcode){ // the opcode cant assign values to imm or zero
			if (rd_is_zero){ // not possible to set values to $zero
			    continue;
			}
			else if (imm_reg_count == 2){ // not possible to set values to $imm
			    continue;
			}
			else
			    op_func->function(rd,rs,rt);
		    }
	    }
	    else
		op_func->function(rd,rs,rt);
	    printf("%d\n",*rd); // debug purpose
	    }
	    return 0;
	}
		
		
	}
}



//int *rd = NULL, rs = 10, rt = 5, *j;
//int var = 20;
//rd = &var;
//func_struct *test;
//test = getFunct("00", 1);
//test->function(rd, &rs, &rt);
//j = getReg("0", 1);
//test->function(j, &rs, &rt);
//return 0;

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
}func_struct;

/* an auxilary struct to be used as a dictionary */
typedef struct { func_struct func; char* str; }func_dict;
func_dict opcode_to_func_dict[] =
{
	{ add, "00"},
	{ sub, "01"},
	{ and, "02"},
	{ or , "03"},
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
typedef struct { int* reg; char* hexa; }reg_dict;
int $zero = 0, $imm = 0, $v0 = 0, $a0 = 0, $a1 = 0, $t0 = 0, $t1 = 0, $t2 = 0, $t3 = 0; //registers
int $s0 = 0, $s1 = 0, $s2 = 0, $gp = 0, $sp = 0, $fp = 0, $ra = 0; //registers
int irq0enable = 0, irq1enable = 0, irq2enable = 0, irqhandler = 0; //hardware registers
int irq0status = 0, irq1status = 0, irq2status = 0, irqreturn = 0; //hardware registers
int clks = 0, leds = 0, reserved = 0, timerenable = 0, timercurrent = 0, timermax = 0; //hardware registers
int diskcmd = 0, disksector = 0, diskbuffer = 0, diskstatus = 0; //hardware registers
int monitorcmd = 0, monitorx = 0, monitory = 0, monitordata = 0; //hardware registers
reg_dict register_from_hexa_dict[] =
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
			for (rSearch = register_from_hexa_dict; rSearch != register_from_hexa_dict + sizeof(register_from_hexa_dict) / sizeof(register_from_hexa_dict[0]); rSearch++)
			{
				if (!strcmp(rSearch->hexa, str))
					selected = rSearch->reg;
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

int main(void)
{
	int *rd, *rs, *rt; // command registers
	int pc = 0, immon = 0, clock = 0, cmdcounter = 0, irq2next, irq; //counters an controll
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
	if (irq == 1)
		choosirq(*irq0enable, *irq0status, *irq1enable, *irq1status, *irq2enable, *irq2status);
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

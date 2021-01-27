#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 256
#define MAXLABELSIZE 50
#define MAXSIZE 500

// all opcode functions below - generic inputs - and when called gets the right inputs according to the type of function (case-switch ahead) 
void add(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = *rs + *rt;
}
void sub(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = *rs - *rt;
}
void and(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = *rs & *rt;
}
void or (int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = *rs | *rt;
}
void xor(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = *rs ^ *rt;
}
void mul(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = *rs * *rt;
}
void sll(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = *rs << *rt;
}
void sra(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io) //Arithmetic shift
{
	*rd = *rs >> *rt;
}
void srl(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)//logical shift
{
	int logical = 0x7FFFFFFF, temp;
	logical = logical >> *rt;
	temp = *rs >> *rt;
	*rd = temp & logical;
}
void beq(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	if (*rs == *rt)
		*pc = *rd;
}
void bne(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	if (*rs != *rt)
		*pc = *rd;
}
void blt(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	if (*rs < *rt)
		*pc = *rd;
}
void bgt(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	if (*rs > *rt)
		*pc = *rd;
}
void ble(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	if (*rs <= *rt)
		*pc = *rd;
}
void bge(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	if (*rs >= *rt)
		*pc = *rd;
}
void jal(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*ra_io = *pc;
	*pc = *rd;
}
void reti(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	return;
}
void lw(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = datamemmory[*rs + *rt];
}
void sw(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	datamemmory[*rs + *rt] = *rd;
}
void in(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	*rd = *io_reg_array[*rs + *rt];
}
void out(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
		*io_reg_array[*rs + *rt] = *rd;
}
void halt(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io)
{
	return;
}


/* auxilary structs to be used for functions */
// funct type for the func_struct
typedef void(*funct)(int* rd, int* rs, int* rt, int datamemmory[4096], int *io_reg_array[22], int* pc, int* ra_io); // generic inputs for all functions


typedef struct func_struct {
	funct function; // the function
	int func_type; // func type between 1-6
}func_struct;



/* an auxilary struct to be used as a dictionary */
typedef struct { func_struct func; char* str; }func_dict; // function-opcode dictionary struct
func_dict opcode_to_func_dict[] =
{
	{ add,1, "00"}, // first 2 fields for func_struct, 3rd for hexa string
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
func_struct* getFunct(char * str, int mode) // gets hexa string and returns a pointer to the funct_struct
{
	func_dict* pSearch;
	func_struct* selected = NULL;
	if (str != NULL)
	{
		switch (mode) // cases for future use
		{
		case 1: // opcode function search
			/* runing on the dictionary to get the opcode selected */
			for (pSearch = opcode_to_func_dict; pSearch != opcode_to_func_dict + sizeof(opcode_to_func_dict) / sizeof(opcode_to_func_dict[0]); pSearch++)
			{
				if (!strcmp(pSearch->str, str))
					selected = &(pSearch->func);
			}
			if (selected == NULL) // not found
				printf("%s is not a valid command name\n", str);
			break;
		}
	}
	else
		printf("expected opcocde name, got NULL\n");
	return selected;
}

///* an auxilary struct to be used as a dictionary */
typedef struct { int* reg; char hexa; }reg_dict; // hexa-register pointer dictionary struct
int* getReg(char str, int mode, reg_dict register_from_hexa_dict[16])
{
	reg_dict rSearch;
	int* selected = NULL;
	if (str != '\0')
	{
		switch (mode) // cases for future use
		{
		case 1: // command search
			/* runing on the dictionary to get the register selected */
			for (int i = 0; i < 16; i++)
			{
				rSearch = register_from_hexa_dict[i];
				if (rSearch.hexa == str) { // true == found
					selected = rSearch.reg;
					break;
				}
			}
			if (selected == NULL)
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
	while (feof(dmemin) == 0)
	{
		fgets(linestr, 10, dmemin); // reads line by line
		linedata = strtol(linestr, NULL, 16); // string to long int
		datamemmory[linenumber] = linedata; // saves the num in the array
		linenumber++;
	}
	return 0;
}

int readimemin(FILE* imemin, char imemmory[1028][10]) { // copy imemin to char array
	int linenumber = 0;
	while (feof(imemin) == 0)
	{
		fgets(imemmory[linenumber], 10, imemin);// reads line by line
		strtok(imemmory[linenumber], "\n"); // replace \n with \0
		linenumber++;
	}
	return 0;
}

int readdiskin(FILE* diskin, int diskmemmory[128][128]) { // copy diskin to matrix
	char linestr[10];
	int linedata, linenumber = 0, sectornumber = 0;
	while (feof(diskin) == 0)
	{
		fgets(linestr, 10, diskin);// reads line by line
		linedata = strtol(linestr, NULL, 16); // string to long int
		diskmemmory[sectornumber][linenumber] = linedata; // saves the num in the sector in the array
		linenumber++;
		if (linenumber == 127) { // reached last line in sector
			sectornumber++;
			linenumber = 0;
		}
	}
	return 0;
}

void printmemmodata(FILE* dmemout, int datamemmory[4096]) { // gets file pointer to dmemout.txt and prints datamemmory to it
	int i;

	for (i = 0; i < 4096; i++) {
		fprintf(dmemout, "%08X\n", datamemmory[i]); //prints to dmemout file
	}
	return;
}

void printdiskmemmory(FILE* diskout, int diskmemmory[128][128]) { //gets file pointer to diskout.txt and prints diskmemmory to it 
	int i,j;

	for (i = 0; i < 128; i++) {
		for (j = 0; j < 128; j++) {
			fprintf(diskout, "%08X\n", diskmemmory[i][j]);
		}
	}
	return;
}

//this function transfer data once every 8 clck cycles
void movedata(int diskcmd, int diskbuffer, int disksector, unsigned int clks, unsigned int endofworkdisk, int diskmemmory[128][128], int datamemmory[4096]) {
	int offset;
	offset = (endofworkdisk - clks) / 16; //detrmine the part of the data that going to be transfer
	if (diskcmd == 1) { // read cmd
		datamemmory[diskbuffer + offset] = diskmemmory[disksector][offset]; 
	}
	else { // write cmd
		diskmemmory[disksector][offset] = datamemmory[diskbuffer + offset];
	}
	return;
}


//this function print into trace file
void printTrace(FILE* trace, int pc, char imemmory[10], int zero, int imm, int v0, int a0, int a1, int t0, int t1, int t2, int t3, int s0, int s1, int s2, int gp, int sp, int fp, int ra) {
	int inst_int;

	inst_int = strtol(imemmory, NULL, 16);
	fprintf(trace, "%03X %05X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n", pc, inst_int, zero, imm, v0, a0, a1, t0, t1, t2, t3, s0, s1, s2, gp, sp, fp, ra);
	return;
}

void initiateregnamelist(char regnamelist[22][15]) { //make a list of the hardware registers name
	strcpy(regnamelist[0], "irq0enable");
	strcpy(regnamelist[1], "irq1enable");
	strcpy(regnamelist[2], "irq2enable");
	strcpy(regnamelist[3], "irq0status");
	strcpy(regnamelist[4], "irq1status");
	strcpy(regnamelist[5], "irq2status");
	strcpy(regnamelist[6], "irqhandler");
	strcpy(regnamelist[7], "irqreturn");
	strcpy(regnamelist[8], "clks");
	strcpy(regnamelist[9], "leds");
	strcpy(regnamelist[10], "reserved");
	strcpy(regnamelist[11], "timerenable");
	strcpy(regnamelist[12], "timercurrent");
	strcpy(regnamelist[13], "timermax");
	strcpy(regnamelist[14], "diskcmd");
	strcpy(regnamelist[15], "disksector");
	strcpy(regnamelist[16], "diskbuffer");
	strcpy(regnamelist[17], "diskstatus");
	strcpy(regnamelist[18], "monitorcmd");
	strcpy(regnamelist[19], "monitorx");
	strcpy(regnamelist[20], "monitory");
	strcpy(regnamelist[21], "monitordata");
	return;
}

void printmonitor(FILE* monitor, FILE* monitoryuv, int monitorbuffer[288][352]) { // prints monitor buffer to monitoryuv and monitor
	int i, j;
	for (i = 0; i < 288; i++) {
		for (j = 0; j < 352; j++) {
			fprintf(monitor, "%02X\n", monitorbuffer[i][j]);
			fprintf(monitoryuv, "%c", monitorbuffer[i][j]);
		}
	}
	return;
}

void printleds(FILE *fleds, int leds, int oldled, unsigned int clks) {
	if (oldled - leds != 0) { // check if there was a change
		if (fleds != NULL) {
			fprintf(fleds, "%u %08X\n", clks, leds); // print next line in leds.txt
		}
	}
	return;
}

//***********************************************************************************************************************************************************************************
																				   //main
//***********************************************************************************************************************************************************************************


int main(int argc, char* argv[]) // *add the arguments for the input*
{
	int zero = 0, imm = 0, v0 = 0, a0 = 0, a1 = 0, t0 = 0, t1 = 0, t2 = 0, t3 = 0; //registers_vars
	int s0 = 0, s1 = 0, s2 = 0, gp = 0, sp = 0, fp = 0, ra = 0; //registers_vars
	int irq0enable = 0, irq1enable = 0, irq2enable = 0, irqhandler = 0; //hardware registers
	int irq0status = 0, irq1status = 0, irq2status = 0, irqreturn = 0; //hardware registers
	int  reserved = 0, timerenable = 0; //hardware registers
	unsigned int leds = 0, clks = -1, timercurrent = 0, timermax = 0; //hardware registers
	unsigned int irq2next = 0, endofworkdisk = 0; // controllers
	int diskcmd = 0, disksector = 0, diskbuffer = 0, diskstatus = 0; //hardware registers
	int monitorcmd = 0, monitorx = 0, monitory = 0, monitordata = 0; //hardware registers
	int *io_reg_array[22] = { &irq0enable, &irq1enable, &irq2enable, &irq0status, &irq1status, &irq2status, &irqhandler, &irqreturn, &clks, &leds, &reserved, &timerenable, &timercurrent, &timermax, &diskcmd, &disksector, &diskbuffer, &diskstatus, &monitorcmd, &monitorx, &monitory, &monitordata };
	reg_dict register_from_hexa_dict[] =  // register pointers- hexa chars dictionary
	{
	{ &zero, '0'},
	{ &imm, '1'},
	{ &v0, '2'},
	{ &a0, '3'},
	{ &a1, '4' },
	{ &t0, '5' },
	{ &t1, '6' },
	{ &t2, '7' },
	{ &t3, '8' },
	{ &s0, '9' },
	{ &s1, 'A' },
	{ &s2, 'B' },
	{ &gp, 'C' },
	{ &sp, 'D' },
	{ &fp, 'E'},
	{ &ra, 'F' },
	};
	int pc = 0, immon = 0, cmdcounter = 0, irq, reti = 0, halt_flag = 0, oldled,irq0ff = 0, irq1ff = 0, irq2ff = 0; //counters and controllers
	FILE *imemin, *dmemin, *diskin, *irq2in;
	FILE *dmemout, *trace, *regout, *hwregtrace, *cycles;
	FILE *fleds, *monitor, *diskout, *monitoryuv;
	char irq2line[MAXSIZE], hwregnamelist[22][15], imemmory[1024][10];
	int monitorbuffer[288][352] = { 0 }, datamemmory[4096] = { 0 }, diskmemmory[128][128] = { 0 }; //initialize arrays for inner memmory and matrix for disk memmory

	initiateregnamelist(hwregnamelist); //initiate hwregnamelist

	imemin = fopen(argv[2], "r"); // read imemin.txt and copy it content into **string** array
	if (imemin != NULL) {
		readimemin(imemin, imemmory);
		fclose(imemin);
	}
	else
		printf("imemin.txt empty or corrupted\n");

	dmemin = fopen(argv[3], "r"); // read dmemin.txt and copy it content into **int** array
	if (dmemin != NULL) {
		readdmemin(dmemin, datamemmory);
		fclose(dmemin);
	}
	else
		printf("dmemin.txt empty or corrupted\n");

	diskin = fopen(argv[4], "r"); // read diskin.txt and copy it content into **int** matrix
	if (diskin != NULL) {
		readdiskin(diskin, diskmemmory);
		fclose(diskin);
	}
	else
		printf("diskin.txt empty or corrupted\n");

	irq2in = fopen(argv[5], "r"); // read irq2in.txt, read and cast the first line into int
	if (irq2in != NULL) {
		fgets(irq2line, MAXSIZE, irq2in);
		irq2next = strtol(irq2line, NULL, 10);
	}
	else
		printf("irq2in.txt empty or corrupted\n");

	fleds = fopen(argv[11], "w");
	if (fleds == NULL)
		printf("Faild to open leds.txt\n");
	else
		fclose(fleds);

	hwregtrace = fopen(argv[9], "w");
	if (hwregtrace == NULL)
		printf("Faild to open hwregtracw.txt\n");

	trace = fopen(argv[8], "w");
	if (trace == NULL)
		printf("Faild to open trace.txt\n");

	//**************************************************strat main loop for reading cmd*************************************************************************
	while (halt_flag == 0 && strcmp(imemmory[pc], "00000")) // terminate if run out of cmnd or halt been activate, not sure about the second if
	{
		irq = (irq0enable && irq0status) | (irq1enable && irq1status) | (irq2enable && irq2status); //irq check
		if (irq == 1 && reti == 0) { //jump to irq handler
			irqreturn = pc;
			pc = irqhandler;
			reti = 1;
		}
		else { //read the  next cmnd
			cmdcounter++; // count this command
			char buffer[MAX_LEN];
			strcpy(buffer, imemmory[pc]);
			// -1 to allow room for NULL terminator for long string
			int rd_is_zero = 0, imm_reg_count = 0, snd_imm_reg_count = 0, trd_imm_reg_count = 0;
			int immediate = 0;
			char * pch = NULL;
			int k = 0;
			char parameters[5]; // init the 5 commands array in the line
			pch = strtok(buffer, " ,\t:"); //cleans current line
			immon = 0;
			while (*pch != '\0') // while not end of string iterates over chars
			{
				if (*pch == '\n') // reached end of line
				{
					*pch = '\0'; // to get out of loop
					continue;
				}
				parameters[k++] = *pch;//assigns the command in the 5 commands 1-line array
				pch++; // gets next char
				if (k > 1) { // k=0,1 are for opcode
					if (*pch == '1') { // saves which register is the immediate one in the current line
						immon = 1;
						if (imm_reg_count) { // counter for case 1 in later switch-case
							if (snd_imm_reg_count) // if a second one is already imm - snd=second
								trd_imm_reg_count = k; // trd=third
							else
								snd_imm_reg_count = k;
						}
						else //first imm reg count 
							imm_reg_count = k;
					}
				}
				if (k == 2)
					if (*pch == '0')
						rd_is_zero = 1; // flag for case 1
			}
			if (irq0ff) {
				irq0status = 1;
				irq0ff = 0;
			}
			if (irq1ff) {
				irq1status = 1;
				irq1ff = 0;
			}
			if (irq2ff) {
				irq2status = 1;
				irq2ff = 0;
			}
			if (clks == irq2next && irq2in != NULL) { //if clks cycle equal to next irq2
				if (irq2enable)
					irq2ff = 1;
				fgets(irq2line, MAXSIZE, irq2in);
				irq2next = strtol(irq2line, NULL, 10);
			}
			if (!((endofworkdisk - clks) % 16) && diskstatus) // after passing 8 cycles from last data transfer and if the transfer did not finished
				movedata(diskcmd, diskbuffer, disksector, clks, endofworkdisk, diskmemmory, datamemmory);
			if (clks == endofworkdisk) {  // finish with the data transfer
				diskcmd = 0;
				diskstatus = 0;
				if(irq1enable)
					irq1ff = 1;
			}
			clks++; // count 1 clock cycle
			if (timerenable) { //if timer is enable count the clock cycle
				timercurrent++;
				if (timercurrent >= timermax) { //if timer reach max - call irq0
					timercurrent = 0;
					if (irq0enable)
						irq0ff = 1;
				}
			}
			if (immon) // immediate flag is true
			{
				immediate = 0;
				char* imm_str = NULL;
				strcpy(buffer, imemmory[pc + 1]);
				//buffer[strcspn(buffer, "\n")] = "\0";
				imm_str = strtok(buffer, " ,\t:");
				immediate = strtol(imm_str, NULL, 16);
				if (immediate > 524287) //check if the number is negative
					imm = immediate - 1048576; //return the real number
				else
					imm = immediate;
				if (irq0ff) { //check irq flipflops and chane their status
					irq0status = 1;
					irq0ff = 0;
				}
				if (irq1ff) {
					irq1status = 1;
					irq1ff = 0;
				}
				if (irq2ff) {
					irq2status = 1;
					irq2ff = 0;
				}
				if (clks == irq2next && irq2in != NULL) {
					if (irq2enable)
						irq2ff = 1;
					fgets(irq2line, MAXSIZE, irq2in);
					irq2next = strtol(irq2line, NULL, 10);
				}
				if (!((endofworkdisk - clks) % 16) && diskstatus)
					movedata(diskcmd, diskbuffer, disksector, clks, endofworkdisk, diskmemmory, datamemmory);
				if (clks == endofworkdisk) { // finish with the data transfer
					diskcmd = 0;
					diskstatus = 0;
					if (irq1enable)
						irq1ff = 1;
				}
				clks++; //count second clock cycle for imm
				if (timerenable) { //if timer is enable count the clock cycle
					timercurrent++;
					if (timercurrent >= timermax) { //if timer reach max - call irq0
						timercurrent = 0;
						if (irq0enable)
							irq0ff = 1;
					}
				}
			}
			int  *rs, *rt, *rd, func_type = 0, is_reti = 0;
			char opcode[3];
			opcode[0] = parameters[0];
			opcode[1] = parameters[1];
			opcode[2] = '\0'; // merge opcode chars to string
			if (trace != NULL)
				printTrace(trace, pc, imemmory[pc], zero, imm, v0, a0, a1, t0, t1, t2, t3, s0, s1, s2, gp, sp, fp, ra); // print next row in trace.txt
			func_struct *op_func;
			op_func = getFunct(opcode, 1); // current opcode function - returns a pointer to funct_struct
			func_type = op_func->func_type; // for the cases
			rd = getReg(parameters[2], 1, register_from_hexa_dict); // gets register from the dictionary
			rs = getReg(parameters[3], 1, register_from_hexa_dict);
			rt = getReg(parameters[4], 1, register_from_hexa_dict);
			if (!strcmp(opcode, "12")) // a reti function
				is_reti = 1;
			if (immon) // if we used imm advance pc by 2
				pc += 2;
			else // advance by 1
				pc++;
			switch (func_type) // type of function - 1-6
			{
			case 1: // // the opcode can't assign values to imm or zero
				if (immon) {
					if (rd_is_zero)// not possible to set values to $zero
						continue;
					else if (imm_reg_count == 2) // not possible to set values to $imm
						continue;
					else {
						op_func->function(rd, rs, rt, NULL, NULL, NULL, NULL);
						break;
					}
				}
				else
					op_func->function(rd, rs, rt, NULL, NULL, NULL, NULL);
				break;
			case 2: // // the opcode with pc
				op_func->function(rd, rs, rt, NULL, NULL, &pc, NULL);
				break;
			case 3: // // reti or jal
				if (is_reti) {
					pc = *io_reg_array[7]; // pc = irqreturn
					reti = 0;
				}
				else // jal
						op_func->function(rd, NULL, NULL, NULL, NULL, &pc, &ra);
				break;
			case 4: // // the opcode uses dmemmory
				op_func->function(rd, rs, rt, datamemmory, NULL, NULL, NULL);
				break;
			case 5: // // the opcode uses IO registers
				if (!strcmp(opcode, "13")) { //if in func
					op_func->function(rd, rs, rt, NULL, io_reg_array, NULL, NULL);
					if (hwregtrace != NULL) {
						fprintf(hwregtrace, "%u READ %s %08X\n", clks, hwregnamelist[*rs + *rt], *io_reg_array[*rs+*rt]); //print next read line in hwregtrace.txt
					}
				}
				else if (!strcmp(opcode, "14")) { //if out func
					if (hwregtrace != NULL) {
						fprintf(hwregtrace, "%u WRITE %s %08X\n", clks, hwregnamelist[*rs + *rt], *rd);//print next write line in hwregtrace.txt
					}
					oldled = leds;
					if (diskstatus && ((*rs + *rt == 14) || (*rs + *rt == 15) || (*rs + *rt == 16)))  // dont take actionif asked to change disk registers while disk is busy
						break;
					else
						op_func->function(rd, rs, rt, NULL, io_reg_array, NULL, NULL);
					if (*rs + *rt == 9) { //if leds command has taken place
						fleds = fopen(argv[11], "a");
						if (fleds != NULL) {
							printleds(fleds, leds, oldled, clks); //print leds
							fclose(fleds);
						}
					}
					else if (monitorcmd == 1) { //change monitor and monitor status
						monitorbuffer[monitory][monitorx] = monitordata;
						monitorcmd = 0;
					}
					else if (diskcmd && !diskstatus) { // start reading or writing
						endofworkdisk = clks + 1024;
						diskstatus = 1;
					}
				}
				break;
			case 6: // // *Halt - stop reading commands. finish printing and execute*
				halt_flag = 1;
				break;
			}
		}
	}//*****************************************************************end of cmnd loop*******************************************************************************

		regout = fopen(argv[7], "w"); // print output file - regout.txt
		if (regout != NULL) {
			fprintf(regout, "%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n", v0, a0, a1, t0, t1, t2, t3);
			fprintf(regout, "%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n", s0, s1, s2, gp, sp, fp, ra);
			fclose(regout);
		}

		cycles = fopen(argv[10], "w"); //print cycles.txt
		if (cycles != NULL) {
			fprintf(cycles, "%u\n%d\n", clks+1, cmdcounter);
			fclose(cycles);
		}

		dmemout = fopen(argv[6], "w"); //print dmemout.txt
		if (dmemout != NULL) {
			printmemmodata(dmemout, datamemmory);
			fclose(dmemout);
		}

		diskout = fopen(argv[14], "w"); //print diskout.txt
		if (diskout != NULL) {
			printdiskmemmory(diskout, diskmemmory);
			fclose(diskout);
		}

		monitor = fopen(argv[12], "w"); //print monitor.txt
		monitoryuv = fopen(argv[13], "wb"); //open binary file monitor.yuv
		if (monitor != NULL) {
			printmonitor(monitor,monitoryuv, monitorbuffer);
			fclose(monitor);
			fclose(monitoryuv);
		}

		if (hwregtrace!=NULL)
			fclose(hwregtrace);
		if (trace != NULL)
			fclose(trace);
		if (irq2in != NULL)
			fclose(irq2in);

		return 0;
}

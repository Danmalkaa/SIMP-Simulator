//
//  main.c
//  SIMP Assembler
//
//  Created by Dan Malka on 28/12/2020.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 256
#define MAXLABELSIZE 50
#define MAXSIZE 500

typedef struct LABEL
{
    char LABELNAME[MAXLABELSIZE];
    int labeladdress;
    struct LABEL* next;
}LABEL;

LABEL* create_label(char label_name[MAXLABELSIZE], int label_address) { //creat new label
    LABEL *new_label;
	char word[5] = "word";

	if (strcmp(word, label_name) == 0)
		return NULL;

    new_label = (LABEL*)malloc(sizeof(LABEL)); //allocate memmory for new label
    if (new_label == NULL) {
        printf("Fail to allocate memmory");
        return NULL;
    }
    strcpy(new_label->LABELNAME, label_name); //give label name
    new_label->labeladdress = label_address; //number of row
    new_label->next = NULL;

    return(new_label);
}

LABEL* first_read(FILE* rfile) {
    char LINE[MAXSIZE]; // a place to put the line and labels
    char strs[6][MAXLABELSIZE]; // a list for the registers name
    int i, l, pc_counter, elements;
    LABEL* label_list = NULL;
    LABEL *current = NULL, *next_label;
    char *ptr, *token;
    const char clean[10] = " :,.-\t\n\r";

    pc_counter = 0;
    while (feof(rfile) == 0) { //read next line untill the end of the file
        fgets(LINE, MAXSIZE, rfile);
        ptr = strchr(LINE, '#'); //clear all string after '#'
        if (ptr != NULL) {
            *ptr = '\0';
        }
        token = strtok(LINE, clean); //break the string into tokens
        elements = 0;
        for (i = 0; i < 6; i++) { // insert the tokens into the list, if there is nothing left insert NULL
            if (token == NULL) {
                strs[i][0] = NULL;
                token = strtok(NULL, clean);
            }
            else {
                strcpy(strs[i], token);
                token = strtok(NULL, clean);
                elements++;
            }
        }
        if (elements == 1) {//there is only label in this row
            if (label_list == NULL) { //insert the first label
                label_list = create_label(strs[0], pc_counter);
                current = label_list;
            }
            else {
                next_label = create_label(strs[0], pc_counter);//insert new label
                current->next = next_label;
                current = current->next;
            }
        }
        else if (elements == 6) { //this is a row with label and command
            if (label_list == NULL) { //insert the first label
                label_list = create_label(strs[0], pc_counter);
                current = label_list;
            }
            else {
                next_label = create_label(strs[0], pc_counter);//insert new label
                current->next = next_label;
                current = current->next;
            }
            pc_counter += 2; //progress the pc counter by 2
        }
        else if (elements == 0)
            continue;
        else {
            l = 0;
            for (i = 0; i < 6; i++) {
                if (strcmp("$imm", strs[i]) == 0) {
                    pc_counter += 2;
                    l = 1;
                    break;
                }
            }
            if (l == 0)
                pc_counter += 1;
        }
    }

    return (label_list);
}

int searchforlabel(LABEL *label_list, char *label_name)
{
    int result = 0, found;
    LABEL *current;

    current = label_list;
    found = 0;
    while (current != NULL)
    {
        if (strcmp(current->LABELNAME, label_name) == 0) {
            found = 1;
            result = current->labeladdress;
            break;
        }
        else {
            current = current->next;
        }
    }
    if (found == 0)
        return (-1);
    else
        return(result);
}

/* an auxilary struct to be used as a dictionary */
typedef struct  { char* str; char* hexa; }dict;

dict register_to_hexa_dict [] =
{
    { "$zero", "0"},
    { "$imm", "1"},
    { "$v0", "2"},
    { "$a0", "3"},
    { "$a1", "4" },
    { "$t0", "5" },
    { "$t1", "6" },
    { "$t2", "7" },
    { "$t3", "8" },
    { "$s0", "9" },
    { "$s1", "A" },
    { "$s2", "B" },
    { "$gp", "C" },
    { "$sp", "D" },
    { "$fp", "E"},
    { "$ra", "F" },
};

/* creating a 'dictionary' for a commands to its hexa lower case only! */
dict command_to_hexa_dict [] =
{
    { "add", "00"},
    { "sub", "01"},
    { "and", "02"},
    { "or", "03"},
    { "xor", "04" },
    { "mul", "05" },
    { "sll", "06" },
    { "sra", "07" },
    { "srl", "08" },
    { "beq", "09" },
    { "bne", "0A" },
    { "blt", "0B" },
    { "bgt", "0C" },
    { "ble", "0D" },
    { "bge", "0E"},
    { "jal", "0F" },
    { "lw", "10" },
    { "sw", "11" },
    { "reti", "12" },
    { "in", "13" },
    { "out", "14" },
    { "halt", "15" },
};

char* getValue(char * str, int mode)
{
    dict* pSearch;
    char * selected = NULL;
    if (str != NULL)
    {
        switch (mode)
        {
            case 1: // command search
                /* runing on the dictionary to get the command selected */
                for(pSearch = command_to_hexa_dict; pSearch != command_to_hexa_dict + sizeof(command_to_hexa_dict) / sizeof(command_to_hexa_dict[0]); pSearch++ )
                {
                    if(!strcmp( pSearch->str, str))
                        selected = (pSearch->hexa);
                }
                if (selected == NULL)
                    printf("%s is not a valid command name\n", str);
                break;
            case 2: // register search
                /* runing on the dictionary to get the register selected */
                for(pSearch = register_to_hexa_dict; pSearch != register_to_hexa_dict + sizeof(register_to_hexa_dict) / sizeof(register_to_hexa_dict[0]); pSearch++ )
                {
                    if(!strcmp( pSearch->str, str))
                        selected = (pSearch->hexa);
                }
                if (selected == NULL)
                    printf("%s is not a valid register name\n", str);
                break;
        }
    }
    else
        printf("expected command/register name, got NULL\n");
    return selected;
}


int main(void)
{
        // Think if we need to free malloc allocated memory
    FILE *fp, *fw_imemin, *dmemin; // check if needed as "a" mode or "w" mode
    fp = fopen("/Users/danmalka/Documents/לימודים/שנה\ ג/סמסטר\ א/מבנה\ המחשב/פרויקט/SIMP\ Assembler/fib.asm", "r");
    if (fp == NULL) {
      perror("Failed: ");
      return 1;
    }
    LABEL *label_list = first_read(fp);
    rewind(fp);
    char buffer[MAXSIZE];
    // -1 to allow room for NULL terminator for really long string
    fw_imemin = fopen("/Users/danmalka/Documents/לימודים/שנה\ ג/סמסטר\ א/מבנה\ המחשב/פרויקט/SIMP\ Assembler/imemin.txt", "w");
    while (fgets(buffer, MAXSIZE - 1, fp))
    {
        int is_imm=0, is_word=0, add_hexa=0, val_hexa=0;
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0; // sets the buffer to 0 after the index for end of line
        char * pch;
        int k = 0;
        char* parameters[5]; // init the 5 commands array in the line
        pch = strtok (buffer," ,\t:");
	if (pch == NULL)
            continue;
        while (pch != NULL)
        {
            if (*pch == '#') // reached comments
            {
              pch = NULL; // to get out of loop
              continue;
            }
            parameters[k++] = pch;//assigns the command in the 5 commands 1-line array
            if (strcmp("$imm", pch) == 0)
                is_imm = 1;
	    if (strcmp(".word", pch) == 0)
                is_word = 1;
            pch = strtok (NULL, " ,\t"); // gets next token
	    if (k == 2)
		if (parameters[1][1] == 'x' || parameters[1][1] == 'X')
		    add_hexa = 1;
	    if (k == 3)
		if (parameters[2][1] == 'x' || parameters[2][1] == 'X')
		    val_hexa = 1;
        }
        int imm,label_line;
        unsigned int imm_5;
        label_line = searchforlabel(label_list, parameters[0]);
        if (label_line!=-1) // check if it is a label line
            continue;
        imm = searchforlabel(label_list, parameters[4]); // check if there is a label in the last token
        if (imm ==-1) // no label
        {
            imm = strtol(parameters[4], NULL, 10);
            imm_5 = (unsigned int)(imm & 0xFFFFF);
        }
        else
        {
            imm_5 = (unsigned int)(imm & 0xFFFFF);
        }
	if (is_word){
            if (add_hexa && val_hexa){
                address = strtol(parameters[1], NULL, 16);
                value = strtol(parameters[2], NULL, 16);
            }
            else if (add_hexa){
                address = strtol(parameters[1], NULL, 16);
                value = strtol(parameters[2], NULL, 10);
            }
            else if (val_hexa){
                value = strtol(parameters[2], NULL, 16);
                address = strtol(parameters[1], NULL, 10);
            }
            else{
                address = strtol(parameters[1], NULL, 10);
                value = strtol(parameters[2], NULL, 10);
            }
	}
        char*  opcode;
        char *rd,*rs,*rt;
	if (!is_word) { // not a .word command
		opcode = getValue(parameters[0], 1);
		rd = getValue(parameters[1], 2);
		rs = getValue(parameters[2], 2);
		rt = getValue(parameters[3], 2);
		fprintf(fw_imemin,"%s%s%s%s\n",opcode,rd,rs,rt); // change to fprintf
		if (is_imm)
		    fprintf(fw_imemin,"%05X\n",imm_5);
	}
	else
            datamemmory[address] = value;
    }
    dmemin = fopen("/Users/danmalka/Documents/לימודים/שנה\ ג/סמסטר\ א/מבנה\ המחשב/פרויקט/SIMP\ Assembler/dmemin.txt", "w");
    for (int i = 0; i<4096; ++i)
        fprintf(dmemin,"%08X\n",datamemmory[i]);
    fclose(fp);
    fclose(dmemin);
    fclose(fw_imemin);
    return 0;
}

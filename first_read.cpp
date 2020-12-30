#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAXSIZE 80
#define MAXLABELSIZE 10

struct LABEL
{
	char LABELNAME[MAXLABELSIZE];
	int labeladdress;
	struct LABEL *next=NULL;
};

LABEL* create_label(char label_name[MAXLABELSIZE], int label_address) { //creat new label
	LABEL new_label;

	new_label = (LABEL*)malloc(sizeof(LABEL)); //allocate memmory for new label
	if (label_list == NULL) {
		printf("Fail to allocate memmory");
		return NULL;
	}
	strcpy(new_label->LABELNAME, label_name); //give label name
	new_label->labeladdress = label_address; //number of row
	new_label->next = NULL;
	
	return(new_label);
}

LABEL* first_read(char* file) {
	FILE *fp;
	char LINE[MAXSIZE], label_name[MAXLABELSIZE]; // a place to put the line and labels
	char strs[6][MAXLABELSIZE]; // a list for the registers name
	int i,j,l,pc_counter,elements;
	LABEL* label_list = NULL;
	LABEL *current = label_list;
	char *ptr, *token;

	fp = fopen(file, "r"); //open file, set fp to be pointer

	if (fp == NULL){
		printf("Unable to read file");
			return NULL;
	}

	pc_counter = 0;
	while (feof == 0) { //read next line untill the end of the file
		fgets(LINE, MAXSIZE, fp);
		ptr = strchr(LINE, '#'); //clear all string after '#'
		if (ptr != NULL) {
			*ptr = '\0';
		}
		token = strtok(LINE, " ,.-"); //break the string into tokens
		elements = 0;
		for (i = 0; i < 6; i++) { // insert the tokens into the list, if there is nothing left insert NULL
			if (token == NULL) {
				strs[i][0] = NULL;
				token = strtok(NULL, s);
			}
			else {
				strcpy(strs[i], token);
				token = strtok(NULL, s);
				elements++;
			}
		}
		if (elements == 1) {//there is only label in this row
			if (label_list == NULL) //insert the first label
				label_list = create_label(strs[0], pc_counter + 1);
			else
				current = create_label(strs[0], pc_counter + 1);//insert new label
		}
		else if (elements == 6) { //this is a row with label and command
			if (label_list == NULL) //insert the first label
				label_list = create_label(strs[0], pc_counter);
			else
				current = create_label(strs[0], pc_counter);//insert new label
			pc_counter = +2; //progress the pc counter
		}
		else {
			l = 0;
			for (i = 0; i < 6; i++) { //check all cells
				if (strcmp("$imm", strs[i]) == 0) { //is there a $imm in the command
					pc_counter = +2;
					l = 1;
					break
				}
			}
			if (l == 0)
				pc_counter = +1;
		}
	}
	fclose(fp);
	return (label_list);
}

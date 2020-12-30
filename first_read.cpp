#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 80
#define MAXLABELSIZE 10

struct LABEL
{
	char LABELNAME[MAXLABELSIZE];
	int labeladdress;
	struct LABEL* next = NULL;
};

LABEL* create_label(char label_name[MAXLABELSIZE], int label_address) { //creat new label
	LABEL *new_label;

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
	const char clean[10] = " :,.-\t\n";

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 80
#define MAXLABELSIZE 10

struct LABEL
{
	char LABELNAME[10];
	long int labeladdress;
	struct LABEL *next=NULL;
};

LABEL* first_read(char* file) {
	FILE *fp;
	char LINE[MAXSIZE], label_name[MAXLABELSIZE]; // a place to put the line and labels
	int i,j,l;
	LABEL* label_list = NULL;
	LABEL *current = label_list;

	fp = fopen(file, "r"); //open file, set fp to be pointer

	if (fp == NULL){
		printf("Unable to read file");
			return NULL;
	}
	
	while (feof == 0) { //read next line untill the end of the file
		fgets(LINE, MAXSIZE, fp);
		for (int i = 0; i < strlen(LINE); ++i) // run until define if there is a Label in this line
			if (LINE[i] == '#') //we reached the comments sector, there will be no label
				break;
			else if (LINE[i] == ':'){ //found a label
				l = 0;
				for (j = 0; j < i; ++j) //copy the label name without white spaces
					if (LINE[j] == ' ' or '\t')
						continue;
					else {
						label_name[l] = LINE[j];
						l++;
					}
				if (label_list == NULL) { //insert the first label
					label_list = (LABEL*)malloc(sizeof(LABEL));
					if (label_list == NULL) {
						printf("Fail to allocate memmory");
						return NULL;
					}
					strcpy(label_list->LABELNAME, label_name);
					label_list->labeladdress = ftell(fp); //address in file in bytes
					*current = *label_list->next;
				}
				else { //insert list of labels
					current = (LABEL*)malloc(sizeof(LABEL));
					if (current == NULL) {
						printf("Fail to allocate memmory");
						return NULL;
					}
					strcpy(current->LABELNAME, label_name);
					current->labeladdress = ftell(fp); //address in file in bytes
					*current = *current->next;
				}
			}
	}
	fclose(fp);
	return (label_list);
}
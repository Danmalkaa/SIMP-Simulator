//
//  proj SIMP.c
//  project
//
//  Created by Dan Malka on 24/12/2020.
//  Copyright © 2020 Dan Malka. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define const1 500
#define const2 (1 << 16)

// input arguments:
// arg1 - what arg1 should contain when the function is called. Is the argument an input argument or a result argument?
// arg2 - what arg2 should contain when the function is called. Is the argument an input argument or a result argument?
//...
// result argument: what is returned? if the return type is void skip this line.
// 1-2 lines of summary on what the function does.
void read_test(FILE *p1, XXXX *head)
{
    team *head1 = head, *team1ptr = NULL, *team2ptr = NULL;
    char line[2*ARR_SIZE+1], temp[ARR_SIZE], temp2[ARR_SIZE], null[ARR_SIZE]={'\0'};
    char team1[ARR_SIZE], team2[ARR_SIZE];
    char c = '0';
    int i,j = 0;
    while (!feof(p3))
    {
        fgets(line, 2*ARR_SIZE+1, p3);
        strtok(line, "\n");
        strtok(line, "\r");
        strncpy(temp, null, sizeof(temp));
        strncpy(temp2, null, sizeof(temp2));
        strncpy(temp, &line[0], 1);
        i=1;
        while (line[i]!= '\0') {
            if (line[i] != ':'){
                strncpy(&c,&line[i],1);
                strcat(temp, &c);
                i++;
            }
            else {
                strcpy(team1,temp);
                i++;
                j=i;
                break;
            }
        }
        strncpy(temp2,&line[j],1);
        j=i+1;
        while (line[j]!= '\0') {
            strncpy(&c,&line[j],1);
            strcat(temp2, &c);
            j++;
        }
        j=0;
        i=0;
        strcpy(team2,temp2);
        team1ptr = find_team(head1, team1);
        team2ptr = find_team(head1, team2);
        playGame(team1ptr, team2ptr);
        strncpy(team1, null, sizeof(team1));
        strncpy(team2, null, sizeof(team2));
    }
}

//
// read file as a chunk
#include <stdio.h>
#include <stdlib.h>

char* ReadFile(char *filename)
{
   char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(filename, "r");

   if (handler)
   {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file
       rewind(handler);

       // Allocate a string that can hold it all
       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

       // Read it all in one operation
       read_size = fread(buffer, sizeof(char), string_size, handler);

       // fread doesn't set it so put a \0 in the last position
       // and buffer is now officially a string
       buffer[string_size] = '\0';

       if (string_size != read_size)
       {
           // Something went wrong, throw away the memory and set
           // the buffer to NULL
           free(buffer);
           buffer = NULL;
       }

       // Always remember to close the file.
       fclose(handler);
    }

    return buffer;
}

int main()
{
    char *string = ReadFile("yourfile.txt");
    if (string)
    {
        char * pch;
        while (pch != NULL)
        {
          printf ("%s\n",pch);
          pch = strtok (NULL, " ,.-");
        }
        puts(string); // Print String/ display it
        gets(string); // get string from user
        free(string);
    }

    return 0;
}

// Split string to tokens
//  if in label dict - return pointer through omer function - replace pointers in string
// ignore comments in row after #
int main ()
{
  char str[] ="- This, a sample string.";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," ,.-");
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ,.-");
  }
  return 0;
}

// Read file and Line by Line
// I/R format - line by line

#include <stdio.h>
#include <string.h>

#define MAX_LEN 256

int main(void)
{
    FILE* fp;
    fp = fopen("file.txt", "r");
    if (fp == NULL) {
      perror("Failed: ");
      return 1;
    }

    char buffer[MAX_LEN];
    // -1 to allow room for NULL terminator for really long string
    while (fgets(buffer, MAX_LEN - 1, fp))
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        printf("%s\n", buffer);
    }

    fclose(fp);
    return 0;
}

// optional - dict for functions - other option is "cases"
// Dictionary for matrix implementation
typedef struct  { int** lines; int isDefined; }mat;
mat matA, matB, matC, matD, matE, matF;

/* an auxilary struct to be used in a dictionary */
typedef struct  { char* str; mat *matrix; }stringToMat;

/* creating a 'dictionary' for a mat name to its mat. lower case only! */
stringToMat matCases [] =
{
    { "mat_a", &matA },
    { "mat_b", &matB },
    { "mat_c", &matC },
    { "mat_d", &matD },
    { "mat_e", &matE },
    { "mat_f", &matF },
};

mat* getMat(char * str)
{
    stringToMat* pCase;
    mat * selected = NULL;
    if (str != NULL)
    {
        /* runing on the dictionary to get the mat selected */
        for(pCase = matCases; pCase != matCases + sizeof(matCases) / sizeof(matCases[0]); pCase++ )
        {
            if(!strcmp( pCase->str, str))
                selected = (pCase->matrix);
        }
        if (selected == NULL)
            printf("%s is not a valid matrix name\n", str);
    }
    else
        printf("expected matrix name, got NULL\n");
    return selected;
}

// Skeleton for Project
int main ()
{
// Read .asm file
    // Replace all constants/Fix - a loop to iterate over strings with delimeters
//Check if I/R Format in Commands dictionary(?)/linked lists
// Convert I/R Command to 16 bits (each and its own structure)
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," ,.-");
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ,.-");
  }
  return 0;
}
// https://github.com/nayefc/mips-simulator/blob/master/simulator.c - Nice implementation for reference!
// https://github.com/rajatkhanduja/MIPS-simulator - another one with documentation etc.

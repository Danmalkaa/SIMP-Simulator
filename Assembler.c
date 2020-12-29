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
#define MAXLABELSIZE 10


void slice(const char *str, char *result, size_t start, size_t end)
{
    strncpy(result, str + start, end - start);
}

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
    { "$t2 ", "7" },
    { "$t3", "8" },
    { "$s0", "9" },
    { "$s1 ", "A" },
    { "$s2 ", "B" },
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
//    char *string = ReadFile("/Users/danmalka/Documents/לימודים/שנה\ ג/סמסטר\ א/מבנה\ המחשב/פרויקט/SIMP\ Assembler/fib.asm");
    FILE* fp;
    fp = fopen("/Users/danmalka/Documents/לימודים/שנה\ ג/סמסטר\ א/מבנה\ המחשב/פרויקט/SIMP\ Assembler/fib.asm", "r");
    if (fp == NULL) {
      perror("Failed: ");
      return 1;
    }

    char buffer[MAX_LEN];
    // -1 to allow room for NULL terminator for really long string
    while (fgets(buffer, MAX_LEN - 1, fp))
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0; // sets the buffer to 0 after the index for end of line
        char * pch;
        int k = 0;
        char* parameters[5]; // init the 5 commands array in the line
        pch = strtok (buffer," ,\t");
        while (pch != NULL)
        {
            if (*pch == '#') // reached comments
            {
              pch = NULL; // to get out of loop
              continue;
            }
            parameters[k++] = pch; //assigns the command in the 5 commands 1-line array
            printf ("%s\n",pch); // just for debug
            pch = strtok (NULL, " ,\t"); // gets next token
        }
        char * to_imemin[5];
        char*  opcode;
        char *rd,*rs,*rt,*to_print;
        opcode = getValue(parameters[0], 1);
        rd = getValue(parameters[1], 2);
        rs = getValue(parameters[2], 2);
        rt = getValue(parameters[3], 2);
        printf("%s%s%s%s\n",opcode,rd,rs,rt); // change to fprintf
        // add here - if == &imm (or rt== "1") another printf \n
    }
    return 0;
}

//
// Created by matt on 7/7/19.
//

#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_command(enum commandtype *type, char* body, int maxlen)
{
    if (maxlen != MAXLEN)
    {
        puts("[Get_command must be provided a buffer of size MAXLEN!]");
        return 1;
    }
    char userinput[MAXLEN];
    prompt_command(userinput, MAXLEN);

    char command[MAXCOMMANDLEN];

    int count = sscanf(userinput, "%" MAXCOMMANDLEN_STR "s %" MAXLEN_STR "s", command, body);

    if (count == 1 && (strcmp(command, "exit") == 0))
    {
        *type = quit;
        return 0;
    }

    if (count != 2)
    {
        puts("[Error parsing user input!]");
        return 1;
    }

    if (strcmp(command, "reg") == 0)
    {
        *type = reg;
    }
    else if (strcmp(command, "dereg") == 0)
    {
        *type = dereg;
    }
    else if (strcmp(command, "send") == 0)
    {
        *type = message;
    }
    else if (strcmp(command, "exit") == 0)
    {
        *type = quit;
    }
    else
    {
        puts("[Invalid command!]");
        return 1;
    }

    return 0;
}

void prompt_command(char* input, int maxlen)
{
    printf(">>> ");
    fgets(input, maxlen, stdin);
}


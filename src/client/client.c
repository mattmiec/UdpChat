//
// Created by Matt Miecnikowski on 2019-07-06.
//

#include <stdlib.h>

#include "../udpchat.h"
#include "shell/shell.h"

int start_client(char* nickname, char* serverip, int serverport, int clientport)
{
    char* buffer = malloc(MAXLEN);
    enum commandtype command;
    while (1)
    {
        int err = get_command(&command, buffer, MAXLEN);
        if (err)
            continue;
        if (command == quit)
            break;
        else if (command == send)
        {
            write_output("[Sending messages is not yet implemented!]");
        }
        else if (command == reg)
        {
            write_output("[Registering is not yet implemented!]");
        }
        else if (command == send)
        {
            write_output("[Deregistering is not yet implemented!]");
        }
    }
    free(buffer);
    return 0;
}

//
// Created by Matt Miecnikowski on 2019-07-06.
//

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../udpchat.h"
#include "shell/shell.h"
#include "sender_toserver.h"
#include "listener.h"
#include "sender_topeer.h"

int start_client(char* nickname, char* serverip, int serverport, int clientport)
{
    int sockfide_out; /// output socket file descriptor
    struct sockaddr_in servaddr; /// server address

    /// create output socket
    if ((sockfide_out = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Failed to create output socket!");
        return 1;
    }

    /// fill in server info
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(serverport);
    servaddr.sin_addr.s_addr = inet_addr(serverip);

    /// send registration request to server
    if (register_client(sockfide_out, servaddr, nickname, clientport))
    {
        write_output("[Failed to register.]");
        return 1;
    }
    write_output("[Welcome, you are registered.]");

    int sockfide_in; /// listener socket file descriptor
    struct sockaddr_in myaddr; /// my address?

    /// create listener socket
    if ((sockfide_in = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Failed to create listener socket!");
        return 1;
    }

    /// setup listener address with provided port
    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(clientport);

    /// bind listener port
    if (bind(sockfide_in, (struct sockaddr*)&myaddr, sizeof(struct sockaddr_in)))
    {
        printf("Failed to bind listener socket!");
        return 1;
    }

    /// start listener thread
    pthread_t listener_thread;
    pthread_create(&listener_thread, NULL, listen_handler, (void*) sockfide_in);

    /// start shell and handle shell commands
    char buffer[MAXLEN];
    enum commandtype command;
    while (1)
    {
        int err = get_command(&command, buffer, MAXLEN);
        if (err)
            continue;
        if (command == quit)
            break;
        else if (command == message)
        {
            send_message(sockfide_out, nickname, buffer);
        }
        else if (command == reg)
        {
            register_client(sockfide_out, servaddr, buffer, clientport);
        }
        else if (command == dereg)
        {
            deregister_client(sockfide_out, servaddr, buffer);
        }
    }
    return 0;
}

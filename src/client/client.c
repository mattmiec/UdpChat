//
// Created by Matt Miecnikowski on 2019-07-06.
//

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../udpchat.h"
#include "shell/shell.h"
#include "sender_toserver.h"
#include "listener.h"
#include "sender_topeer.h"
#include "../usertable.h"

int start_client(char* nickname, char* serverip, int serverport, int clientport)
{
    int sockfide; /// socket file descriptor
    struct sockaddr_in myaddr; /// my address
    bool acked = false;

    if (initialize_user_table())
    {
        puts(">>> [Failed to initialize user table!]");
        return 1;
    }

    /// create socket
    if ((sockfide = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        puts(">>> [Failed to create listener socket!]");
        return 1;
    }

    /// setup listener address with provided port
    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(clientport);

    /// bind listener port
    if (bind(sockfide, (struct sockaddr*)&myaddr, sizeof(struct sockaddr_in)))
    {
        puts(">>> [Failed to bind listener socket!]");
        return 1;
    }

    struct sockaddr_in servaddr; /// server address

    /// fill in server info
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(serverport);
    servaddr.sin_addr.s_addr = inet_addr(serverip);


    /// start listener thread
    pthread_t listener_thread;
    struct listen_handler_args args;
    args.socketfide = sockfide;
    args.acked = &acked;
    pthread_create(&listener_thread, NULL, listen_handler, (void*) &args);

    printf(">>> ");
    /// send registration request to server
    if (register_client(sockfide, servaddr, nickname, nickname, &acked))
    {
        return 1;
    }

    /// start shell and handle shell commands
    char buffer[MAXLEN];
    enum commandtype command;
    while (1)
    {
        int err = get_command(&command, buffer, MAXLEN);
        printf(">>> ");
        if (err)
            continue;
        if (command == quit)
            break;
        else if (command == message)
        {
            send_message(sockfide, nickname, buffer, &acked, servaddr);
        }
        else if (command == reg)
        {
            register_client(sockfide, servaddr, nickname, buffer, &acked);
        }
        else if (command == dereg)
        {
            deregister_client(sockfide, servaddr, nickname, buffer, &acked);
        }
    }
    return 0;
}

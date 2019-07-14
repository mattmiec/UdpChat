//
// Created by matt on 7/12/19.
//

#include "listener.h"
#include <arpa/inet.h>
#include "../packets.h"
#include "shell/shell.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sender_topeer.h"
#include "../usertable.h"
#include <unistd.h>

void *listen_handler(void* args)
{
    int socketfide = ((struct listen_handler_args*)args)->socketfide;
    bool* acked = ((struct listen_handler_args*)args)->acked;

    struct packet inpacket;
    struct sockaddr_in sockaddr;
    socklen_t fromlen = sizeof(sockaddr);
    while (1)
    {
        int bytes = recvfrom(socketfide, (void *)&inpacket, sizeof(inpacket), 0, (struct sockaddr*)&sockaddr, &fromlen);

        if (strcmp(inpacket.type, "MESSAGE") == 0)
        {
            char* buffer = malloc(strlen(inpacket.fromname) + strlen(": ") + strlen(inpacket.message));
            strcpy(buffer, inpacket.fromname);
            strcat(buffer, ": ");
            strcat(buffer, inpacket.message);
            send_ack(socketfide, inpacket.toname, inpacket.fromname);

            usleep(600000);
            puts(buffer);
            printf(">>> ");
            fflush(stdout);
            free(buffer);
        }

        if (strcmp(inpacket.type, "ACK") == 0)
        {
            *acked = true;
        }

        if (strcmp(inpacket.type, "TABLE") == 0)
        {
            usleep(600000);
            memcpy(user_table, inpacket.message, sizeof(user_table));
            printf("[Client table updated.]\n>>> ");
            fflush(stdout);
        }

        if (bytes < 0)
        {
            usleep(600000);
            puts("[Listener failure. Exiting.]");
            printf(">>> ");
            fflush(stdout);
            return 1;
        }
    }
}

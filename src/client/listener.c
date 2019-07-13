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
            puts(buffer);
            printf(">>> ");
            fflush(stdout);
            free(buffer);

            send_ack(socketfide, inpacket.toname, inpacket.fromname);
        }

        if (strcmp(inpacket.type, "ACK") == 0)
        {
            *acked = true;
        }

        if (bytes < 0)
        {
            return 1;
        }
    }
}

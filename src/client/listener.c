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
    struct sockaddr_in fromaddr;
    socklen_t fromlen = sizeof(fromaddr);
    while (1)
    {
        int bytes = recvfrom(socketfide, (void *)&inpacket, sizeof(inpacket), 0, (struct sockaddr*)&fromaddr, &fromlen);

        if (strcmp(inpacket.type, "MESSAGE") == 0)
        {
            ///char* buffer = malloc(strlen(inpacket.fromname) + strlen(": ") + strlen(inpacket.message));
            ///strcpy(buffer, inpacket.fromname);
            ///strcat(buffer, ": ");
            ///strcat(buffer, inpacket.message);

            /// write output
            ///puts(buffer);
            printf("%s: %s\n>>> ", inpacket.fromname, inpacket.message);
            fflush(stdout);

            ///free(buffer);

            send_ack(socketfide, inpacket.toname, inpacket.fromname);
            /// construct and send ack packet
            struct packet outpacket;
            memset(&outpacket, 0, sizeof(outpacket));
            strncpy(outpacket.type, "ACK", strlen("ACK"));
            int bytes_sent = sendto(
                    socketfide,
                    &outpacket,
                    sizeof(outpacket),
                    0,
                    (const struct sockaddr*)&fromaddr,
                    sizeof(fromaddr)
            );
            if (bytes_sent < 0)
                return 1;

        }

        if (strcmp(inpacket.type, "ACK") == 0)
        {
            *acked = true;
        }

        if (strcmp(inpacket.type, "TABLE") == 0)
        {
            /// delay here just to avoid out-of-order UI messages
            usleep(500000);
            /// update user status table
            memcpy(user_table, inpacket.message, sizeof(user_table));
            printf("[Client table updated.]\n>>> ");
            fflush(stdout);
        }

        if (bytes < 0)
        {
            puts("[Listener failure. Exiting.]");
            printf(">>> ");
            fflush(stdout);
            return 1;
        }
    }
}

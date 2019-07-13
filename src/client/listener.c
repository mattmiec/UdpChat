//
// Created by matt on 7/12/19.
//

#include "listener.h"
#include <arpa/inet.h>
#include "../packets.h"
#include "shell/shell.h"
#include <string.h>
#include <stdlib.h>


void *listen_handler(void* socketfide)
{
    struct packet inpacket;
    struct sockaddr_in sockaddr;
    socklen_t fromlen;
    while (1)
    {
        int bytes = recvfrom((int)socketfide, (void *)&inpacket, sizeof(inpacket), 0, (struct sockaddr*)&sockaddr, &fromlen);

        if (strcmp(inpacket.type, "MESSAGE") == 0)
        {
            char* buffer = malloc(strlen(inpacket.fromname) + strlen(": ") + strlen(inpacket.message));
            strcpy(buffer, inpacket.fromname);
            strcat(buffer, ": ");
            strcat(buffer, inpacket.message);
            write_output(buffer);
            free(buffer);

            /// send ack
            struct packet outpacket;
            memset(&outpacket, 0, sizeof(outpacket));
            strncpy(outpacket.type, "ACK", strlen("ACK"));
            int bytes_sent = sendto(
                    (int)socketfide,
                    &outpacket,
                    sizeof(outpacket),
                    0,
                    (const struct sockaddr*)&sockaddr,
                    sizeof(sockaddr)
            );
            if (bytes_sent < 0)
                return 1;

        }

        if (bytes < 0)
        {
            return 1;
        }
    }
}

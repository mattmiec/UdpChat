//
// Created by matt on 7/12/19.
//

#include "../packets.h"
#include "sender_toserver.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int register_client(int socketfide_out, struct sockaddr_in servaddr, char* nickname, char* cname, bool* acked)
{
    if (strcmp(nickname, cname) != 0)
    {
       printf("[You may only register with your previous username.]\n>>> ");
       return 0;
    }
    struct packet outpacket;
    memset(&outpacket, 0, sizeof(outpacket));
    strncpy(outpacket.type, "REG", strlen("REG"));
    strncpy(outpacket.fromname, nickname, strlen(nickname));
    int bytes_sent = sendto(
            socketfide_out,
            &outpacket,
            sizeof(outpacket),
            0,
            (const struct sockaddr*)&servaddr,
            sizeof(servaddr)
            );
    if (bytes_sent < 0)
    {
        printf("[ERROR SENDING REGISTRATION]\n>>> ");
    }
    usleep(500000);
    if (*acked)
    {
        *acked=false;
        printf("[Welcome, you are registered.]\n>>> ");
        return 0;
    }
    printf("[No ack from server, you are NOT registered.]\n>>> ");
    return 0;
}

int deregister_client(int socketfide_out, struct sockaddr_in servaddr, char* nickname, char* cname, bool* acked)
{
    if (strcmp(nickname, cname) != 0)
    {
        printf("[You may only deregister with your current username.]\n>>> ");
        return 0;
    }
    struct packet outpacket;
    memset(&outpacket, 0, sizeof(outpacket));
    strncpy(outpacket.type, "DEREG", strlen("DEREG"));
    strncpy(outpacket.fromname, nickname, strlen(nickname));
    int bytes_sent = sendto(
            socketfide_out,
            &outpacket,
            sizeof(outpacket),
            0,
            (const struct sockaddr*)&servaddr,
            sizeof(servaddr)
    );
    if (bytes_sent < 0)
    {
        printf("[ERROR SENDING DEREGISTRATION]\n>>> ");
    }
    int count = 0;
    while (count < 5)
    {
        usleep(500000);
        if (*acked)
        {
            *acked=false;
            printf("[You are offline. Bye.]\n>>> ");
            return 0;
        }
        count++;
    }
    printf("[No ack from server, you are NOT registered.]\n>>> ");
    printf("[Server not responding]\n>>> ");
    printf("[Exiting]");
    exit(1);
}

int offline_message(int socketfide_out, struct sockaddr_in servaddr, char* myname, char* toname, char* message)
{
    struct packet outpacket;
    memset(&outpacket, 0, sizeof(outpacket));
    strncpy(outpacket.type, "MESSAGE", strlen("MESSAGE"));
    strncpy(outpacket.fromname, myname, strlen(myname));
    strncpy(outpacket.toname, toname, strlen(toname));
    strncpy(outpacket.message, message, strlen(message));
    int bytes_sent = sendto(
            socketfide_out,
            &outpacket,
            sizeof(outpacket),
            0,
            (const struct sockaddr*)&servaddr,
            sizeof(servaddr)
    );
    if (bytes_sent < 0) {
        printf("[Failed to send to server]\n>>> ");
        printf("[Exiting]");
        exit(1);
    }
    printf("[Message received by the server and saved]\n>>> ");
    return 0;
}

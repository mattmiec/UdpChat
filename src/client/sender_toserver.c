//
// Created by matt on 7/12/19.
//

#include "../packets.h"
#include "sender_toserver.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int register_client(int socketfide_out, struct sockaddr_in servaddr, char* myname, int listenerport)
{
    struct packet outpacket;
    memset(&outpacket, 0, sizeof(outpacket));
    outpacket.ackport = 0;
    outpacket.listenerport = listenerport;
    strncpy(outpacket.type, "REGISTER", strlen("REGISTER"));
    strncpy(outpacket.fromname, myname, strlen(myname));
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
        puts(">>> [Failed to register.]");
        return 1;
    }
    puts(">>> [Welcome, you are registered.]");
    return 0;
}

int deregister_client(int socketfide_out, struct sockaddr_in servaddr, char* myname)
{
    struct packet outpacket;
    memset(&outpacket, 0, sizeof(outpacket));
    outpacket.ackport = 0;
    strncpy(outpacket.type, "DEREG", strlen("DEREG"));
    strncpy(outpacket.fromname, myname, strlen(myname));
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
        puts(">>> [Server not responding]");
        puts(">>> [Exiting]");
        exit(1);
    }
    puts(">>> [You are offline. Bye.]");
    return 0;
}

int offline_message(int socketfide_out, struct sockaddr_in servaddr, char* myname, char* toname, char* message)
{
    struct packet outpacket;
    memset(&outpacket, 0, sizeof(outpacket));
    outpacket.ackport = 0;
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
        puts(">>> [Failed to send to server]");
        puts(">>> [Exiting]");
        exit(1);
    }
    puts(">>> [Message received by the server and saved]");
    return 0;
}

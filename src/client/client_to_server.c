//
// Created by matt on 7/12/19.
//

#include "../packets.h"
#include "client_to_server.h"
#include <arpa/inet.h>
#include <string.h>

int register_client(int socketfide_out, struct sockaddr_in servaddr, char* myname, int listenerport)
{
    struct server_packet outpacket;
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
        return 1;
    return 0;
}

int deregister_client(int socketfide_out, struct sockaddr_in servaddr, char* myname)
{
    struct server_packet outpacket;
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
        return 1;
    return 0;
}

int offline_message(int socketfide_out, struct sockaddr_in servaddr, char* myname, char* toname, char* message)
{
    struct server_packet outpacket;
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
    if (bytes_sent < 0)
        return 1;
    return 0;
}

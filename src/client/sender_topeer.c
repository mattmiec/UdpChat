//
// Created by matt on 7/12/19.
//

#include "sender_topeer.h"
#include "../packets.h"
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "../usertable.h"
#include "shell/shell.h"
#include "sender_toserver.h"
#include <stdlib.h>
#include <unistd.h>

int send_message(int socketfide_out, char* myname, char* sendcommand, bool* acked, struct sockaddr_in servaddr)
{
    char toname[LENNAME];
    char message[LENMESSAGE];

    int count = sscanf(sendcommand, "%16s %2048s", toname, message);

    /// fill in peer info
    struct sockaddr_in peeraddr;
    struct table_entry peer_table_entry = lookup_table_entry(toname);
    memset(&peeraddr, 0, sizeof(peeraddr));
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(peer_table_entry.port);
    peeraddr.sin_addr.s_addr = peer_table_entry.ip_addr;


    /// construct and send message packet
    struct packet outpacket;
    memset(&outpacket, 0, sizeof(outpacket));
    outpacket.ackport = 0;
    strncpy(outpacket.type, "MESSAGE", strlen("MESSAGE"));
    strncpy(outpacket.fromname, myname, strlen(myname));
    strncpy(outpacket.toname, toname, strlen(toname));
    strncpy(outpacket.message, message, strlen(message));
    *acked = false;
    count++;
    int bytes_sent = sendto(
            socketfide_out,
            &outpacket,
            sizeof(outpacket),
            0,
            (const struct sockaddr*)&peeraddr,
            sizeof(peeraddr)
    );
    if (bytes_sent < 0)
    {
        printf(">>> [Failed to send message to %s.]\n", toname);
        return 1;
    }
    usleep(500000);
    if (*acked)
    {
        *acked=false;
        printf(">>>[Message received by %s.]\n", toname);
        return 0;
    }
    printf(">>> [No ACK from %s, message sent to server.]\n", toname);
    offline_message(socketfide_out, servaddr, myname, toname, message);
    return 0;

}

int send_ack(int socketfide_out, char* myname, char* toname)
{
    /// fill in peer info
    struct sockaddr_in peeraddr;
    struct table_entry peer_table_entry = lookup_table_entry(toname);
    memset(&peeraddr, 0, sizeof(peeraddr));
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(peer_table_entry.port);
    peeraddr.sin_addr.s_addr = peer_table_entry.ip_addr;

    /// construct and send ack packet
    struct packet outpacket;
    memset(&outpacket, 0, sizeof(outpacket));
    strncpy(outpacket.type, "ACK", strlen("ACK"));
    int bytes_sent = sendto(
            socketfide_out,
            &outpacket,
            sizeof(outpacket),
            0,
            (const struct sockaddr*)&peeraddr,
            sizeof(peeraddr)
    );
    if (bytes_sent < 0)
        return 1;
}

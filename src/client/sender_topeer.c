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

int send_message(int socketfide_out, char* myname, char* sendcommand)
{
    char toname[LENNAME];
    char message[LENMESSAGE];

    int count = sscanf(sendcommand, "%16s %2048s", toname, message);

    struct sockaddr_in peeraddr; /// server address

    struct table_entry peer_table_entry = lookup_table_entry(toname);

    /// fill in peer info
    memset(&peeraddr, 0, sizeof(peeraddr));
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(peer_table_entry.port);
    peeraddr.sin_addr.s_addr = peer_table_entry.ip_addr;

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
            (const struct sockaddr*)&peeraddr,
            sizeof(peeraddr)
    );
    if (bytes_sent < 0)
    {
        printf(">>> [Failed to send message to %s.]", toname);
        return 1;
    }
    printf(">>> [Sent message to %s.]", toname);
    return 0;
}

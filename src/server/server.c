//
// Created by Matt Miecnikowski on 2019-07-06.
//

#include "../udpchat.h"
#include <arpa/inet.h>
#include "../packets.h"
#include "../usertable.h"
#include <stdio.h>
#include <string.h>

int start_server(int port)
{
    int sockfide; /// socket file descriptor
    struct sockaddr_in myaddr; /// my address

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
    myaddr.sin_port = htons(port);

    /// bind listener port
    if (bind(sockfide, (struct sockaddr*)&myaddr, sizeof(struct sockaddr_in)))
    {
        puts(">>> [Failed to bind listener socket!]");
        return 1;
    }

    puts("SERVER SUCCESSFULLY STARTED");

    while (1)
    {
        struct packet inpacket;
        struct packet outpacket;
        struct sockaddr_in clientaddr;
        socklen_t fromlen = sizeof(clientaddr);
        int bytes = recvfrom(sockfide, (void *)&inpacket, sizeof(inpacket), 0, (struct sockaddr*)&clientaddr, &fromlen);

        printf("RECEIVED PACKET FROM %s\n", inpacket.fromname);

        if (strcmp(inpacket.type, "REG") == 0)
        {
            /// first check that username is not a duplicate)
            bool duplicate = false;
            struct table_entry check_table_entry = lookup_table_entry(inpacket.fromname);
            if (strcmp(check_table_entry.nickname, inpacket.fromname) == 0)
            {
                /// if we're here then there an entry exists, but we don't know if this is the user re-registering
                /// or if it's another user attempting to register with the same username
                /// use ip and port to distinguish
                if ((clientaddr.sin_addr.s_addr != check_table_entry.ip_addr)
                   || (ntohs(clientaddr.sin_port) != check_table_entry.port))
                {
                    duplicate = true;
                }
            }
            if (duplicate)
            {
                /// if it's a duplicate, just stop here, client will not receive ack and will know it's not registered
                continue;
            }
            /// then update user table
            struct table_entry new_table_entry;
            memset(&new_table_entry, 0, sizeof(new_table_entry));
            strcpy(new_table_entry.nickname, inpacket.fromname);
            new_table_entry.ip_addr = clientaddr.sin_addr.s_addr;
            new_table_entry.port = ntohs(clientaddr.sin_port);
            new_table_entry.status = true;
            if (upsert_table_entry(new_table_entry))
                return 1;
            printf("REGISTERED USER %s\n", inpacket.fromname);
            /// construct and send ack packet
            memset(&outpacket, 0, sizeof(outpacket));
            strncpy(outpacket.type, "ACK", strlen("ACK"));
            int bytes_sent = sendto(
                    sockfide,
                    &outpacket,
                    sizeof(outpacket),
                    0,
                    (const struct sockaddr*)&clientaddr,
                    sizeof(clientaddr)
            );
            if (bytes_sent < 0)
                return 1;
            /// then iterate through registered users and sent them updated table
            memset(&outpacket, 0, sizeof(outpacket));
            strncpy(outpacket.type, "TABLE", strlen("TABLE"));
            memcpy(outpacket.message, user_table, sizeof(user_table));
            for (int i = 0; i < MAXUSERS; i++)
            {
                if (user_table[i].nickname[i] != '\0')
                {
                    /// fill in peer info
                    memset(&clientaddr, 0, sizeof(clientaddr));
                    clientaddr.sin_family = AF_INET;
                    clientaddr.sin_port = htons(user_table[i].port);
                    clientaddr.sin_addr.s_addr = user_table[i].ip_addr;

                    int bytes_sent = sendto(
                            sockfide,
                            &outpacket,
                            sizeof(outpacket),
                            0,
                            (const struct sockaddr*)&clientaddr,
                            sizeof(clientaddr)
                    );
                    if (bytes_sent < 0)
                        return 1;
                }
            }
        }

        if (strcmp(inpacket.type, "DEREG") == 0)
        {
            /// first update user table
            struct table_entry new_table_entry;
            memset(&new_table_entry, 0, sizeof(new_table_entry));
            strcpy(new_table_entry.nickname, inpacket.fromname);
            new_table_entry.ip_addr = clientaddr.sin_addr.s_addr;
            new_table_entry.port = ntohs(clientaddr.sin_port);
            new_table_entry.status = false;
            if (upsert_table_entry(new_table_entry))
                return 1;
            printf("REGISTERED USER %s\n", inpacket.fromname);
            /// construct and send ack packet
            memset(&outpacket, 0, sizeof(outpacket));
            strncpy(outpacket.type, "ACK", strlen("ACK"));
            int bytes_sent = sendto(
                    sockfide,
                    &outpacket,
                    sizeof(outpacket),
                    0,
                    (const struct sockaddr*)&clientaddr,
                    sizeof(clientaddr)
            );
            if (bytes_sent < 0)
                return 1;
            /// then iterate through registered users and sent them updated table
            memset(&outpacket, 0, sizeof(outpacket));
            strncpy(outpacket.type, "TABLE", strlen("TABLE"));
            memcpy(outpacket.message, user_table, sizeof(user_table));
            for (int i = 0; i < MAXUSERS; i++)
            {
                if (user_table[i].nickname[i] != '\0')
                {
                    /// fill in peer info
                    memset(&clientaddr, 0, sizeof(clientaddr));
                    clientaddr.sin_family = AF_INET;
                    clientaddr.sin_port = htons(user_table[i].port);
                    clientaddr.sin_addr.s_addr = user_table[i].ip_addr;

                    int bytes_sent = sendto(
                            sockfide,
                            &outpacket,
                            sizeof(outpacket),
                            0,
                            (const struct sockaddr*)&clientaddr,
                            sizeof(clientaddr)
                    );
                    if (bytes_sent < 0)
                        return 1;
                }
            }
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

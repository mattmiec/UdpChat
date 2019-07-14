//
// Created by matt on 7/12/19.
//

#ifndef UDPCHAT_USERTABLE_H
#define UDPCHAT_USERTABLE_H

#include "packets.h"
#include <stdbool.h>
#include <arpa/inet.h>

#define TABLEFILENAME "user_table.txt"
#define MAXUSERS 16 /// max 16 users so that user_table will fit in message field of packet

struct table_entry
{
    char nickname[LENNAME];
    in_addr_t ip_addr;
    int port;
    bool status;
};

struct table_entry user_table[MAXUSERS]; //define in header so that can be read or overwritten easily

int initialize_user_table();
struct table_entry lookup_table_entry(char* nickname);
int upsert_table_entry(struct table_entry);

#endif //UDPCHAT_USERTABLE_H

//
// Created by matt on 7/12/19.
//

#ifndef UDPCHAT_USERTABLE_H
#define UDPCHAT_USERTABLE_H

#include "packets.h"
#include <stdbool.h>
#include <arpa/inet.h>

#define TABLEFILENAME "user_table.txt"

struct table_entry
{
    char nickname[LENNAME];
    in_addr_t ip_addr;
    int port;
    bool status;
};

struct table_entry lookup_table_entry(char* nickname);
int insert_table_entry(struct table_entry);

#endif //UDPCHAT_USERTABLE_H

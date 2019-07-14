//
// Created by matt on 7/12/19.
//

#include <stdio.h>
#include <string.h>
#include "usertable.h"

int initialize_user_table()
{
    memset(&user_table, 0, sizeof(user_table));
    return 0;
}

struct table_entry lookup_table_entry(char* nickname)
{
    for (int i = 0; i < MAXUSERS; i++)
    {
        if (strcmp(user_table[i].nickname, nickname) == 0)
        {
            return user_table[i];
        }
    }
    struct table_entry blank_table_entry;
    memset(&blank_table_entry, 0, sizeof(blank_table_entry));
    return blank_table_entry;
}

int upsert_table_entry(struct table_entry my_table_entry)
{
    for (int i = 0; i < MAXUSERS; i++)
    {
        /// check for matching record
        /// or blank record
        if ((strcmp(user_table[i].nickname, my_table_entry.nickname) == 0)
            || (user_table[i].nickname[0] == '\0'))
        {
            user_table[i] = my_table_entry;
            return 0;
        }
    }
    return 1; /// if haven't returned already, table is full
}

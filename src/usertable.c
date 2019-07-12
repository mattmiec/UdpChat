//
// Created by matt on 7/12/19.
//

#include <stdio.h>
#include <string.h>
#include "usertable.h"

struct table_entry lookup_table_entry(char* nickname)
{
    FILE *fptr = fopen(TABLEFILENAME, "r");
    struct table_entry my_table_entry;
    memset(&my_table_entry, 0, sizeof(my_table_entry));
    char name[LENNAME];
    in_addr_t ip_addr;
    char ip_addr_str[15];
    int port;
    int status;
    int read = 1;
    while (1)
    {
        read = fscanf(fptr, "%s %s %d %d", name, ip_addr_str, &port, &status);
        if (read != 4)
        {
            fclose(fptr);
            return my_table_entry;
        }
        if (strcmp(name, nickname) == 0)
        {
            strcpy(my_table_entry.nickname, name);
            my_table_entry.ip_addr = inet_addr(ip_addr_str);
            my_table_entry.port = port;
            my_table_entry.status = (bool) status;
            fclose(fptr);
            return my_table_entry;
        }
    }
}

int insert_table_entry(struct table_entry my_table_entry)
{
    FILE *fptr = fopen(TABLEFILENAME, "a");
    struct in_addr my_in_addr;
    my_in_addr.s_addr = my_table_entry.ip_addr;
    if (fprintf(fptr,
            "%s %s %d %d",
            my_table_entry.nickname,
            inet_ntoa(my_in_addr),
            my_table_entry.port,
            (int)my_table_entry.status) < 0)
    {
        fclose(fptr);
        return 1;
    }
    fclose(fptr);
    return 0;



}

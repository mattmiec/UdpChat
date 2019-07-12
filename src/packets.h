//
// Created by matt on 7/12/19.
//

#ifndef UDPCHAT_PACKETS_H
#define UDPCHAT_PACKETS_H

#include <stdint.h>

#define LENTYPE 8
#define LENNAME 16
#define LENMESSAGE 2048

struct server_packet
{
    int32_t ackport;
    int32_t listenerport;
    char type[LENTYPE]; // packet type
    char fromname[LENNAME]; // nickname of user sending request
    char toname[LENNAME]; // (optional) nickname of message recipient if packet type is "OFFLINE"
    char message[LENMESSAGE]; // (optional) message text if packet type is "OFFLINE"
};

struct peer_packet
{
    char fromname[LENNAME];
    char message[LENMESSAGE];
};

#endif //UDPCHAT_PACKETS_H

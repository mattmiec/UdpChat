//
// Created by matt on 7/12/19.
//

#ifndef UDPCHAT_LISTENER_H
#define UDPCHAT_LISTENER_H

#include <stdbool.h>

struct listen_handler_args
{
    int socketfide;
    bool* acked;
};

void *listen_handler(void* args);

#endif //UDPCHAT_LISTENER_H

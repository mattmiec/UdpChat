//
// Created by matt on 7/12/19.
//

#ifndef UDPCHAT_SENDER_TOPEER_H
#define UDPCHAT_SENDER_TOPEER_H

#include <stdbool.h>
#include <arpa/inet.h>

int send_message(int socketfide_out, char* myname, char* sendcommand, bool* acked, struct sockaddr_in servaddr);
int send_ack(int socketfide_out, char* myname, char* toname);

#endif //UDPCHAT_SENDER_TOPEER_H

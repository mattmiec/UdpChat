//
// Created by matt on 7/12/19.
//

#ifndef UDPCHAT_SENDER_TOSERVER_H
#define UDPCHAT_SENDER_TOSERVER_H

#include <arpa/inet.h>
#include <stdbool.h>

int register_client(int socketfide_out, struct sockaddr_in servaddr, char* nickname, char* cname, bool* acked);
int deregister_client(int socketfide_out, struct sockaddr_in servaddr, char* nickname, char* cname, bool* acked);
int offline_message(int socketfide_out, struct sockaddr_in servaddr, char* myname, char* toname, char* message);

#endif //UDPCHAT_SENDER_TOSERVER_H

//
// Created by matt on 7/12/19.
//

#ifndef UDPCHAT_CLIENT_TO_SERVER_H
#define UDPCHAT_CLIENT_TO_SERVER_H

#include <arpa/inet.h>

int register_client(int socketfide_out, struct sockaddr_in, char* myname, int listenerport);
int deregister_client(int socketfide_out, struct sockaddr_in, char* myname);
int offline_message(int socketfide_out, struct sockaddr_in, char* myname, char* toname, char* message);

#endif //UDPCHAT_CLIENT_TO_SERVER_H

//
// Created by Matt Miecnikowski on 2019-07-06.
//

#ifndef UDPCHAT_UDPCHAT_H
#define UDPCHAT_UDPCHAT_H

// start server
// return 1 on failure to start
// else return 0
int start_server(int port);

// start client
// return 1 on failure to start
// else return 0
int start_client(char* nickname, char* serverip, int serverport, int clientport);

#endif //UDPCHAT_UDPCHAT_H

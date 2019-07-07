#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udpchat.h"

int main(int argc, char** argv) {
    if (argc < 3)
    {
        printf("Usage: UdpChat {[-c <nickname> <serverip> <serverport> <clientport>] | [-s <serverport]}\n");
        return 1;
    }
    if (strcmp(argv[1], "-c") == 0)
    {
        if (argc < 6)
        {
            printf("Usage: UdpChat {[-c <nickname> <serverip> <serverport> <clientport>] | [-s <serverport]}\n");
            return 1;
        }
        char* nickname = argv[2];
        char* serverip = argv[3];
        int serverport = atoi(argv[4]);
        int clientport = atoi(argv[5]);
        if (start_client(nickname, serverip, serverport, clientport) != 0)
        {
            printf("Error starting client.");
            return 1;
        }
    }
    else if (strcmp(argv[1], "-s") == 0)
    {
        int port = atoi(argv[2]);
        if (start_server(port) != 0)
        {
            printf("Error starting client.");
            return 1;
        }
    }
    else
    {
        printf("Usage: UdpChat {[-c <nickname> <serverip> <serverport> <clientport>] | [-s <serverport]}\n");
        return 1;
    }
    return 0;
}
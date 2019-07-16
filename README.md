#UdpChat
- mtm2163
- Matt Miecnikowski
- CSEE S4119 Computer Networks
- PA1 - UdpChat

##Overview
- UdpChat is a simple command-line chat application built using udp sockets
- To start a client: ./UdpChat -c <nick-name> <server-ip> <server-port> <client-port>
- To start a server: ./UdpChat -s <server-port>
- To send a message: send <nick-name> message...
- To deregister (go offline): dereg <nick-name>
- To reregister (go back online): reg <nick-name>
- To exit: exit (or ctrl-c)
- The UdpChat server stores messages while you are offline, you will receive them when you reregister
- Note: UdpChat is a connectionless application and will not be reliable on lossy networks

##How to Build
- If you have not receieved a distribution with a makefile, you must first create one by executing: cmake
<path-to-udpchat>
- When you have a makefile, simply execute: make

##Implementation Details
- UdpChat uses fixed size udp packets to communicate between clients and server
- Each client binds a socket to the specified port and sends and receives all messages via this socket
- The server also uses one socket bound to the specified port for all communication
- Each client uses two threads
    - The UI thread waits for user input, then processes commands and sends messages to clients or server as appropriate
    - The listener thread waits for packets from other clients or the server and processes them appropriately
    - The only interthread communication is a single boolean variable used by the listener to tell the UI that outgoing 
    messages have been acked
- Clients and server both store the user status table as a fixed size in memory array (the maximum number of users is
currently hardcoded to 16)
- To save messages for offline users, the server creates a file <clientname>.msg and stores each message as a row
    - This file is deleted once the user reregisters and receives the messages

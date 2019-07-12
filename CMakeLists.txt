cmake_minimum_required(VERSION 3.14)
project(UdpChat C)

set(CMAKE_C_STANDARD 99)

add_executable(UdpChat src/main.c src/udpchat.h src/server/server.c src/client/client.c src/client/shell/shell.c src/client/shell/shell.h src/packets.h src/client/client_to_server.c src/client/client_to_server.h)
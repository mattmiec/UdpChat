cmake_minimum_required(VERSION 2.8)
project(UdpChat C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -std=c99")

add_executable(UdpChat src/main.c src/udpchat.h src/server/server.c src/client/client.c src/client/shell/shell.c src/client/shell/shell.h src/packets.h src/client/sender_toserver.c src/client/sender_toserver.h src/client/listener.c src/client/listener.h src/usertable.c src/usertable.h src/client/sender_topeer.c src/client/sender_topeer.h)

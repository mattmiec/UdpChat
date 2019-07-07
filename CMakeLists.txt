cmake_minimum_required(VERSION 3.14)
project(UdpChat C)

set(CMAKE_C_STANDARD 99)

add_executable(UdpChat src/main.c src/udpchat.h src/server/server.c src/client/client.c)
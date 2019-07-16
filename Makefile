CC=gcc
CFLAGS=-pthread -std=c99
csrc = $(wildcard src/*.c) \
       $(wildcard src/client/*.c) \
       $(wildcard src/server/*.c) \
       $(wildcard src/client/shell/*.c)
obj = $(csrc:.c=.o)

CC = gcc
CFLAGS = -pthread -std=c99

UdpChat: $(obj)
	$(CC) $(CFLAGS) -o $@ $^

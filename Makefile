CC      = gcc
CFLAGS  = -Wall
LDFLAGS = -loveit -lcrypto

all: msend mreceive

msend: msend.c msend.h
	$(CC) $(LDFLAGS) -o msend msend.c

mreceive: mreceive.c mreceive.h
	$(CC) $(LDFLAGS) -o mreceive mreceive.c

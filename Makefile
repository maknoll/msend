CC      = gcc
LDFLAGS = -lcrypto
CFLAGS  = -arch x86_64
OBJ     = main.o send.o receive.o

all: $(OBJ)
	$(CC) $(LDFLAGS) -o mm $(OBJ)

main: main.c
	$(CC) $(CFLAGS) -c main.c

send: send.c send.h protocol.h
	$(CC) $(CFLAGS) -c send.c

receive: receive.c receive.h protocol.h
	$(CC) $(CFLAGS) -c receive.c

clean:
	rm -f $(OBJ) mm

gui:
	xcodebuild -project msend-gui.xcodeproj $(XCODEFLAGS)

guiclean:
	xcodebuild -project msend-gui.xcodeproj clean;

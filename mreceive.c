#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mreceive.h"

#define BUFFERSIZE 1024

int main (int argc, char * argv[]) 
{
	char buffer[BUFFERSIZE];
	int bytes;
	int port = atoi(argv[1]);
	struct mheader header;

	int socket = socket_create_listen("0.0.0.0", port, 1);
	
	char clientIp[16];
	int clientPort;
	int client = socket_accept(socket, clientIp, &clientPort);

	printf("%s %i", clientIp, clientPort);

	printf("%i", sizeof(struct mheader));
	socket_read(client, &header, sizeof(struct mheader), -1);

	printf("%s %i", header.filename, header.length);

	int file = open(header.filename, O_WRONLY);

	int i;
	for (i = header.length; i > 0; i - BUFFERSIZE)
	{
		bytes =  i > BUFFERSIZE ? BUFFERSIZE : i;
		socket_read(client, buffer, bytes, -1);
		write(file, buffer, bytes);
	}

	close(file);
	socket_close(client);
	socket_close(socket);
	
	printf("success");

	return 0;
}

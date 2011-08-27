#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

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

	recv(client, &header, sizeof(struct mheader), 0);

	printf("%s %i \n", header.filename, header.length);

	int file = open(header.filename, O_WRONLY| O_TRUNC | O_CREAT, 0644);
	if(file < 0)
	{
		perror("open");
		return -1;
	}

	int r = 0;
	int i = header.length;
	while(i > 0)
	{
		//r = socket_read(client, buffer, BUFFERSIZE, -1);
		r = recv(client, buffer, BUFFERSIZE, 0);
		printf("%i \n", r);
		if(write(file, buffer, r) < 0)
		{
			perror("write");
			return -1;
		}
		i -= r;
	}

	close(file);
	socket_close(client);
	socket_close(socket);
	
	return 0;
}

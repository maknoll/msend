#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "mreceive.h"

#define BUFFERSIZE 1452

int main (int argc, char * argv[]) 
{
	char buffer[BUFFERSIZE];
	int bytes;
	int port = atoi(argv[1]);
	struct mheader header;

	int socket = socket_create_listen("0.0.0.0", port, 4);
	
	struct sockaddr_in si;
	socklen_t len = sizeof(si);

	while (true)
	{
		int client = accept(socket, (void *)&si, &len);

		read(client, &header, sizeof(struct mheader));

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
			r = read(client, buffer, BUFFERSIZE);
			if(write(file, buffer, r) < 0)
			{
				perror("write");
				return -1;
			}
			i -= r;
		}

		close(file);
		socket_close(client);
		printf("%s received (%i bytes)\n", header.filename, header.length);
	}

	socket_close(socket);
	
	return 0;
}

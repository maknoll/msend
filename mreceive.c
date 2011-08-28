#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <openssl/sha.h>

#include "mreceive.h"

#define BUFFERSIZE 1452

int main (int argc, char * argv[]) 
{
	char buffer[BUFFERSIZE];
	int port = atoi(argv[1]);
	struct mheader header;
	SHA256_CTX sha_ctx;
	unsigned char sha256[SHA256_DIGEST_LENGTH];
	unsigned char sha256_client[SHA256_DIGEST_LENGTH];

	int socket = socket_create_listen("0.0.0.0", port, 4);
	
	struct sockaddr_in si;
	socklen_t len = sizeof(si);

	while (true)
	{
		int client = accept(socket, (void *)&si, &len);
		if(client < 0)
		{
			perror("accept");
			return -1;
		}

		if(read(client, &header, sizeof(struct mheader)) < 0)
		{
			perror("read");
			return -1;
		}

		int file = open(header.filename, O_WRONLY| O_TRUNC | O_CREAT, 0644);
		if(file < 0)
		{
			perror("open");
			return -1;
		}

		SHA256_Init(&sha_ctx);

		int i, bytes;
		for(i = header.length, bytes = 0; i > 0; i -= bytes)
		{
<<<<<<< HEAD
			bytes = read(client, buffer, BUFFERSIZE);
			if(bytes < 0)
			{
				perror("read");
				return -1;
			}

=======
			bytes = read(client, buffer, i > BUFFERSIZE ? BUFFERSIZE : i);
>>>>>>> 35e9a611a8384bf54a3bf25530b084d573b8ad72
			if(write(file, buffer, bytes) < 0)
			{
				perror("write");
				return -1;
			}
<<<<<<< HEAD
=======

			SHA256_Update(&sha_ctx, (unsigned char*)buffer, bytes);

			i -= bytes;
>>>>>>> 35e9a611a8384bf54a3bf25530b084d573b8ad72
		}

		close(file);

		read(client, sha256_client, SHA256_DIGEST_LENGTH);

		SHA256_Final(sha256, &sha_ctx);

		socket_close(client);

		printf("%s received (%i bytes) ", header.filename, header.length);
		if (memcmp(sha256, sha256_client, SHA256_DIGEST_LENGTH) == 0)
			printf("SHA256 matches\n");
		else
			printf("SHA256 differs\n");

	}

	socket_close(socket);
	
	return 0;
}

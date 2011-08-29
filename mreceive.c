#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <netdb.h>
#include <string.h>
#include <openssl/sha.h>

#include "mreceive.h"

#define BUFFERSIZE 1452

int socket_bind_listen(char *port)
{
	struct addrinfo hints, *info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
//	hints.ai_family = AF_INET;

	int error = getaddrinfo(NULL, port, &hints, &info);
	if (error) {
		errx(1, "%s", gai_strerror(error));
	}

	int sock = socket(info->ai_family, info->ai_socktype, 0);

	if (bind(sock, info->ai_addr, info->ai_addrlen))
	{
		perror("bind");
		return -1;
	}

	listen(sock, 4);

	freeaddrinfo(info);

	return sock;
}

int main (int argc, char * argv[]) 
{
	char buffer[BUFFERSIZE];
	char *port = argv[1];
	struct mheader header;
	SHA256_CTX sha_ctx;
	unsigned char sha256[SHA256_DIGEST_LENGTH];
	unsigned char sha256_client[SHA256_DIGEST_LENGTH];

	int sock = socket_bind_listen(port);

	while (true)
	{
		int client = accept(sock, NULL, NULL);
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

		long i;
		int bytes;
		for(i = header.length, bytes = 0; i > 0; i -= bytes)
		{
			bytes = read(client, buffer, i > BUFFERSIZE ? BUFFERSIZE : i);
			if(bytes < 0)
			{
				perror("read");
				return -1;
			}
			
			if(write(file, buffer, bytes) < 0)
			{
				perror("write");
				return -1;
			}

			SHA256_Update(&sha_ctx, (unsigned char*)buffer, bytes);
		}

		close(file);

		if(read(client, sha256_client, SHA256_DIGEST_LENGTH) < 0)
		{
			perror("read");
			return -1;
		}

		SHA256_Final(sha256, &sha_ctx);

		close(client);

		printf("%s received (%li bytes) ", header.filename, header.length);
		if (!memcmp(sha256, sha256_client, SHA256_DIGEST_LENGTH))
			printf("SHA256 matches\n");
		else
			printf("SHA256 differs\n");

	}

	close(sock);
	
	return 0;
}

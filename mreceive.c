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
		return -1;

	listen(sock, 4);

	freeaddrinfo(info);

	return sock;
}

int receive_file(int client)
{
	struct mheader header;
	char buffer[BUFFERSIZE];
	SHA256_CTX sha_ctx;
	unsigned char sha256[SHA256_DIGEST_LENGTH];
	unsigned char sha256_client[SHA256_DIGEST_LENGTH];

	if(client < 0)
		return -1;

	if(read(client, &header, sizeof(struct mheader)) < 0)
		return -1;

	int file = open(header.filename, O_WRONLY| O_TRUNC | O_CREAT, 0644);
	if(file < 0)
		return -1;

	SHA256_Init(&sha_ctx);

	long i;
	int bytes;
	for(i = header.length, bytes = 0; i > 0; i -= bytes)
	{
		fflush(stdout);
		printf("\r%s %li/%li bytes ", header.filename, header.length - i, header.length);

		bytes = read(client, buffer, i > BUFFERSIZE ? BUFFERSIZE : i);
		if(bytes < 0)
			return -1;
		
		if(write(file, buffer, bytes) < 0)
			return -1;

		SHA256_Update(&sha_ctx, (unsigned char*)buffer, bytes);
	}

	close(file);

	if(read(client, sha256_client, SHA256_DIGEST_LENGTH) < 0)
		return -1;

	SHA256_Final(sha256, &sha_ctx);

	close(client);

	printf("\r%s %li bytes received ", header.filename, header.length);
	if (!memcmp(sha256, sha256_client, SHA256_DIGEST_LENGTH))
		printf("SHA256 matches\n");
	else
		printf("SHA256 differs\n");

	return 0;
}

int main (int argc, char * argv[]) 
{
	char *port = argv[1];
	int client;

	int sock = socket_bind_listen(port);
	if(sock < 0)
	{
		perror("socket_bind_listen");
		return -1;
	}

	while (true)
	{
		client = accept(sock, NULL, NULL);
		if(client < 0)
		{
			perror("accept");
			return -1;
		}
		
		if(receive_file(client) < 0)
		{
			perror("receive_file");
			return -1;
		}
	}

	close(sock);
	
	return 0;
}

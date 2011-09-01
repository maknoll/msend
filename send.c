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

#include "send.h"
#include "protocol.h"

#define BUFFERSIZE 1452

long get_filesize(char *filename)
{
	struct stat fileinfo;
	stat(filename, &fileinfo);
	
	return fileinfo.st_size;
}

int socket_connect(char *addr, char *port)
{
	int sock;
	struct addrinfo hints, *res, *info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_family = AF_INET;

	int error = getaddrinfo(addr, port, &hints, &res);
	if (error) {
		errx(1, "%s", gai_strerror(error));
	}
	
	for(info = res; info != NULL; info = info->ai_next)
	{
		sock = socket(info->ai_family, info->ai_socktype, 0);
		if(connect(sock, (struct sockaddr*)info->ai_addr, info->ai_addrlen) == 0)
			break;
		else
			close(sock);
	}

	freeaddrinfo(info);

	return sock;
}

int send_file(int sock, char *filename)
{
	struct mheader header;
	char buffer[BUFFERSIZE];
	SHA256_CTX sha_ctx;
	unsigned char sha256[SHA256_DIGEST_LENGTH];
	
	memset(&header.filename, 0, 128);

	strcpy(header.filename, filename);
	header.length = get_filesize(filename);

	int file = open(filename, O_RDONLY);
	if(file < 0)
		return -1;
	
	if(write(sock, &header, sizeof(struct mheader)) < 0)
		return -1;
	
	SHA256_Init(&sha_ctx);

	long i;
	int bytes;
	for (i = header.length, bytes = 0; i > 0; i -= bytes)
	{
		fflush(stdout);
		printf("\r%s %li/%li bytes ", header.filename, header.length - i, header.length);

		bytes = read(file, buffer, BUFFERSIZE);
		if(bytes < 0)
			return -1;

		if(write(sock, buffer, bytes) < 0)
			return -1;
		
		SHA256_Update(&sha_ctx, (unsigned char*)buffer, bytes);
	}

	printf("\r%s %li bytes send       \n", header.filename, header.length);
	close(file);

	SHA256_Final(sha256, &sha_ctx);
	if(write(sock, sha256, SHA256_DIGEST_LENGTH) < 0)
		return -1;

	return 0;
}

int send_to (char *addr, char *port, char *filename) 
{
	int sock = socket_connect(addr, port);
	if(sock < 0)
	{
		perror("socket_connect");
		return -1;
	}

	if(send_file(sock, filename) < 0)
	{
		perror("send_file");
		return -1;
	}

	close(sock);

	return 0;
}

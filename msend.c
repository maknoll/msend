#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <openssl/sha.h>

#include "msend.h"

#define BUFFERSIZE 1452

long getFileSize(char *filename)
{
	struct stat fileinfo;
	stat(filename, &fileinfo);
	
	return fileinfo.st_size;
}

int socket_connect(char *addr, char *port)
{
	struct addrinfo hints, *info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;

	int error = getaddrinfo(addr, port, &hints, &info);
	if (error) {
		errx(1, "%s", gai_strerror(error));
	}

	int sock = socket(info->ai_family, info->ai_socktype, 0);

	connect(sock, (struct sockaddr*)info->ai_addr, info->ai_addrlen);
	
	freeaddrinfo(info);

	return sock;
}

int main (int argc, char *argv[]) 
{
	char *addr = argv[1];
	char *port = argv[2];
	char *filename = argv[3];
	char buffer[BUFFERSIZE];
	struct mheader header;
	SHA256_CTX sha_ctx;
	unsigned char sha256[SHA256_DIGEST_LENGTH];

	memset(&header.filename, 0, 128);

	strcpy(header.filename, filename);
	header.length = getFileSize(filename);

	int sock = socket_connect(addr, port);

	int file = open(filename, O_RDONLY);
	if(file < 0)
	{
		perror("open");
		return -1;
	}
	
	if(write(sock, &header, sizeof(struct mheader)) < 0)
	{
		perror("socket_write");
		return -1;
	}
	
	SHA256_Init(&sha_ctx);

	long i;
	int bytes;
	for (i = header.length, bytes = 0; i > 0; i -= bytes)
	{
		bytes = read(file, buffer, BUFFERSIZE);
		if(bytes < 0)
		{
			perror("read");
			return -1;
		}

		if(write(sock, buffer, bytes) < 0)
		{
			perror("socket_write");
		}
		
		SHA256_Update(&sha_ctx, (unsigned char*)buffer, bytes);
	}

	close(file);

	SHA256_Final(sha256, &sha_ctx);
	if(write(sock, sha256, SHA256_DIGEST_LENGTH) < 0)
	{
		perror("socket_write");
		return -1;
	}

	close(sock);

	return 0;
}

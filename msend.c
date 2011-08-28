#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <openssl/sha.h>

#include "msend.h"

#define BUFFERSIZE 1452

int getFileSize(char *filename)
{
	struct stat fileinfo;
	stat(filename, &fileinfo);
	
	return fileinfo.st_size;
}

int main (int argc, char *argv[]) 
{
	char *ip = argv[1];
	int port = atoi(argv[2]);
	char *filename = argv[3];
	char buffer[BUFFERSIZE];
	struct mheader header;
	SHA256_CTX sha_ctx;
	unsigned char sha256[SHA256_DIGEST_LENGTH];

	// initialize array with zeros
	memset(&header.filename, 0, 128);

	strcpy(header.filename, filename);
	header.length = getFileSize(filename);

	int socket = socket_create_connect(ip, port);
	int file = open(filename, O_RDONLY);
	if(file < 0)
	{
		perror("open");
		return -1;
	}
	
	if(socket_write(socket, &header, sizeof(struct mheader)) < 0)
	{
		perror("socket_write");
		return -1;
	}
	
	SHA256_Init(&sha_ctx);

	int i, bytes;
	for (i = header.length, bytes = 0; i > 0; i -= bytes)
	{
		bytes = read(file, buffer, BUFFERSIZE);
<<<<<<< HEAD
		if(bytes < 0)
		{
			perror("read");
			return -1;
		}

		if(socket_write(socket, buffer, bytes) < 0)
		{
			perror("socket_write");
		}
=======
		socket_write(socket, buffer, bytes);
		
		SHA256_Update(&sha_ctx, (unsigned char*)buffer, bytes);

		i -= bytes;
>>>>>>> 35e9a611a8384bf54a3bf25530b084d573b8ad72
	}

	close(file);

	SHA256_Final(sha256, &sha_ctx);
	socket_write(socket, sha256, SHA256_DIGEST_LENGTH);

	socket_close(socket);

	return 0;
}

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "msend.h"

#define BUFFERSIZE 1452

int getFileSize(char *filename)
{
	struct stat fileinfo;
	stat(filename, &fileinfo);
	
	return fileinfo.st_size;
}

int main (int argc, char * argv[]) 
{
	char *ip = argv[1];
	int port = atoi(argv[2]);
	char *filename = argv[3];
	char buffer[BUFFERSIZE];
	struct mheader header;

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
	
	socket_write(socket, &header, sizeof(struct mheader));
	
	int i, bytes;
	for (i = header.length, bytes = 0; i > 0;)
	{
		bytes = read(file, buffer, BUFFERSIZE);
		socket_write(socket, buffer, bytes);
		
		i -= bytes;
	}

	close(file);
	socket_close(socket);

	return 0;
}

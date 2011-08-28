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
	int bytes;
	struct mheader header;

	strcpy(header.filename, filename);
	header.length = getFileSize(filename);

	int socket = socket_create_connect(ip, port);
	int file = open(filename, O_RDONLY);
	
	socket_write(socket, &header, sizeof(struct mheader));
		
	while ((bytes = read(file, buffer, BUFFERSIZE)) > 0)
	{
		socket_write(socket, buffer, bytes);
	}

	close(file);
	socket_close(socket);

	return 0;
}

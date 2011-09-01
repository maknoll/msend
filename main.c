#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "send.h"
#include "receive.h"

int main (int argc, char *argv[]) 
{
	if (!strcmp(argv[1], "send"))
	{
		char *addr = argv[2];
		char *port = argv[3];
		char *filename = argv[4];

		send_to (addr, port, filename);
	}
	
	if (!strcmp(argv[1], "receive"))
	{
		char *port = argv[2];

		receive_from (port);
	}
}

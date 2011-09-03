#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "send.h"
#include "receive.h"

void print_usage(void)
{
	printf("Usage: mm receive|send [<args>]\n");
}

int main (int argc, char *argv[]) 
{
	char default_port[] = "8080";

	if (argc < 2)
	{
		print_usage();
		return 0;
	}

	if (!strcmp(argv[1], "send"))
	{
		if (argc < 4)
		{
			print_usage();
			return 0;
		}

		char *addr;
		char *port;
		char *filepath;

		if (argc >= 5)
		{
			addr = argv[2];
			port = argv[3];
			filepath = argv[4];
		}
		else
		{
			addr = argv[2];
			port = default_port;
			filepath = argv[3];
		}

		send_to (addr, port, filepath);
	}
	
	if (!strcmp(argv[1], "receive"))
	{
		char *port;

		if (argc >= 3)
			port = argv[2];
		else
			port = default_port;

		receive_from (port, NULL);
	}

	return 1;
}

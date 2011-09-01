struct mheader 
{
	char filename[128];
	long length;
};

long getFileSize(char *filename);

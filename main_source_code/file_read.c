#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define FILE_NAME "sample.txt"

void main(int argc,char **argv)
{
	char data_buff[1024]={0};
	if(argc==2)
	{

	}
	else
	{
	 printf("NO file Input\n");
	 exit(0);
	}
	char *filename=argv[1];
	printf("Program Started File Name =%s\n",filename);	
	FILE *file_ptr;

	while(1)
	{
		file_ptr=fopen(filename,"r+");
		fread(data_buff,1024,1,file_ptr);
		printf("read Data=%s\n",data_buff);
		sleep(1);
	}



}

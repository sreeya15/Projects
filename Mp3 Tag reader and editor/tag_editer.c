#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
int str_edit(char *hexstr)
{
	int result = 0;
	for(int i=0;i<4;i++)
	{
		result <<= 8;
		result |= hexstr[i];
	}
	return result;
}
int editing(FILE *ed,char *argv[] )
{
	int size = strlen(argv[3]);
	char *str = argv[3],str1[4] = {'\0'};
	fread(str1,1,sizeof(int),ed);
	int size1 =str_edit(str1);
	fseek(ed,2,SEEK_CUR);
	fwrite(str,1,size,ed);
	char ch = 0x00;
	for(int i = 0;i< (size1 - size);i++)
		fwrite(&ch,1,1,ed);
	return e_success;
}
int edit_tag(char *argv[])
{
	int size=0;
	char str[4] = {'\0'};
	FILE *fptr = NULL;
	if(strstr(argv[4],".mp3"))
	{
		fptr = fopen(argv[4],"r+");
		if(fptr == NULL)
		{
			printf("Error opening file: %s\n",argv[2]);
			return e_failure;
		}
	}
	fseek(fptr,10,SEEK_CUR);
	if(strcmp("-t",argv[2]) == 0)
	{
		fread(str,1,4,fptr);
		str[4] = '\0';
		if(strcmp(str,"TIT2") == 0)
		{
			if(editing(fptr,argv) == e_success)
			{
				printf("----------Change the Title----------\n\n");
				printf("\nTitle\t: %s\n",argv[3]);
				return e_success;
			}
		}
	}
	if(strcmp("-a",argv[2]) == 0)
	{
		fseek(fptr,57,SEEK_CUR);
		char str[4];
		fread(str,1,4,fptr);
		if(strcmp(str,"TPE1") == 0)
		{
			if(editing(fptr,argv) == e_success)
			{
				printf("----------Change the Artist----------\n\n");
				printf("\nArtist\t: %s\n",argv[3]);
				return e_success;
			}
		}
	}
	if(strcmp("-A",argv[2]) == 0)
	{
		fseek(fptr,100,SEEK_CUR);
		fread(str,1,4,fptr);
		str[4] ='\0';
		if(strcmp(str,"TALB") == 0)
		{
			if(editing(fptr,argv) == e_success)
			{
				printf("----------Change the Album----------\n\n");
				printf("\nAlbum\t: %s\n",argv[3]);
				return e_success;
			}
		}
	}
	if(strcmp("-y",argv[2]) == 0)
	{
		fseek(fptr,119,SEEK_CUR);
		fread(str,1,4,fptr);
		str[4] ='\0';
		printf("%s\n",str);
		if(strcmp(str,"TYER") == 0)
		{
			int size = strlen(argv[3]);
			char *st = argv[3],str1[4] = {'\0'};
			fread(str1,1,sizeof(int),fptr);
			int size1 =str_edit(str1);
			fseek(fptr,3,SEEK_CUR);
			fwrite(st,1,size,fptr);
			printf("----------Change the year----------\n\n");
			printf("\nYear\t: %s\n",argv[3]);
			return e_success;
		}
	}
	if(strcmp("-m",argv[2]) == 0)
	{
		fseek(fptr,134,SEEK_CUR);
		fread(str,1,4,fptr);
		str[4] ='\0';
		if(strcmp(str,"TCON") == 0)
		{
			if(editing(fptr,argv) == e_success)
			{
				printf("----------Change the content----------\n\n");
				printf("\nContent\t: %s\n",argv[3]);
				return e_success;
			}
		}
	}
	if(strcmp("-c",argv[2]) == 0)
	{
		fseek(fptr,175,SEEK_CUR);
		fread(str,1,4,fptr);
		str[4] ='\0';
		if(strcmp(str,"COMM") == 0)
		{
			if(editing(fptr,argv) == e_success)
			{
				printf("----------Change the comment----------\n\n");
				printf("\nComment\t: %s\n",argv[3]);
				return e_success;
			}
		}
	}
	else
	{
		return e_failure;
	}
}

/*
Name: Trouble M
Description:
Date:
Sample I/P:
Sample O/P:
 */
#include <stdio.h>
#include <string.h>
#include "types.h"

int view_tager (FILE *r);
int str_hex(char *hexstr);
int str_edit(char *hexstr);
int editing (FILE *ed,char *argv[]);
int edit_tag(char *argv[]);

char str[4],*str1="ID3";
int size;
int main(int argc,char *argv[])
{
	FILE *fptr;
	if(argc > 2 && argc <= 5)
	{
		if(strcmp("-v",argv[1]) == 0)
		{
			if(strstr(argv[2],".mp3"))
			{
				fptr = fopen(argv[2],"r");
				if(fptr == NULL)
				{
					printf("Error opening file: %s\n",argv[2]);
					return 0;
				}
				fread(str,1,3,fptr);              //Read first 3 bytes (ID3)
				if(strcmp(str,str1) == 0)         // check whether its equal to str1 = ID3
				{
					fread(str,1,2,fptr);      

					if(str[0] == 3 && (str[1] == 0))
					{
						fseek(fptr, 5, SEEK_CUR);
						if(view_tager(fptr) == e_success)
						{

						}
					}
					else
					{
						printf("Pass Version 3.0 \n");
					}
				}
				else
				{
					printf("Pass MP3 file version should be ID3\n");
				}

			}
			else
			{
				printf("In command line pass the mp3 file\n");
			}
		}
		else if(strcmp("-e",argv[1]) == 0)
		{
			printf("-------------------------SELECTED EDIT DETAILS---------------------------\n\n\n");
			printf("----------SELECTED EDIT OPTIONS----------\n\n");
			if(edit_tag(argv) == e_success)
			{
				printf("\n\n----------SELECTED TAG EDITED SUCCESSFULLY----------\n");
			}
			else
			{
				printf("Pass Valid arguments like -t,-a,-A,-y,-m,-c\n");
			}
		}
		else
		{
			printf("Pass the argument like -v -e \n");
		}
	}
	else if(argc == 2)
	{
		if(strcmp(argv[1],"--help") == 0)
		{
			printf("--------------------------------------Help Menu-----------------------------------------\n");
			printf("1. -v -> to view mp3 file contents\n2. -e -> to edit mp3 file contents\n\t2.1. -t -> to edit song title\n\t2.2. -a -> to edit artist name\n\t2.3. -A -> to edit album name\n\t2.4. -y -> to edit year\n\t2.5. -m -> to edit content\n\t2.6. -c -> to edit comment\n");
			printf("-----------------------------------------------------------------------------------------\n");
		}
	}
	else
	{
		printf("ERROR: ./a.out : Invalid Arguments\nUsage:\nTo view please pass like:  ./a.out -v mp3filename\nTo edit please pass like:  ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\nTo get help pass like:  ./a.out --help\n");
	}
}


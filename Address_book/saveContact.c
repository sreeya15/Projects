#include "address_book.h"

void saveContact(struct Contact** addressBook, int* numContacts)
{
	FILE *f=NULL;
	f = fopen("file.csv","w+");
	if(f == NULL)
	{
		fprintf(stderr,"Unable to open the file\n");
	}
	char choice;
	printf("\n\nEnter the \'N\' to Ignore or \'Y\' to Save: ");
	getchar();
	scanf("%c",&choice);
	if(choice == 'Y' || choice == 'y')
	{
		for(int i =0;i<*numContacts;i++)
			fprintf(f,"%s %s %s\n",(*addressBook)[i].name,(*addressBook)[i].phoneNumber,(*addressBook)[i].email);
	}
}

#include "address_book.h"

void listContact(struct Contact** addressBook, int* numContacts)
{
	if(*numContacts != 0)
	{
		printf("\n#### All Contacts ####\n\nS.no\t|Name\t\t\t|Phone Number\t\t|Email id\t\n");
		for(int i=0;i<*numContacts;i++)
		{
			printf("\n%d\t %s\t\t\t%s\t\t%s\n",i+1,(*addressBook)[i].name,(*addressBook)[i].phoneNumber,(*addressBook)[i].email);
		}
		char c;
		printf("\nSelect \'L\' to List or \'q\' to cancel: ");
		getchar();
		scanf("%c",&c);
		if(c == 'L' || c == 'l')
			editContact(addressBook, numContacts);
	}
	else
		printf("No Contact was found\n");
}

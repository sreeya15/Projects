#include "address_book.h"

void addContact(struct Contact** addressBook, int* numContacts) 
{
	(*numContacts)++;
	*addressBook = (struct Contact*)realloc(*addressBook, (*numContacts) * sizeof(struct Contact));
	(*addressBook)[*numContacts - 1].name = (char*)calloc(100 , sizeof(char));
	(*addressBook)[*numContacts - 1].phoneNumber = (char*)calloc(20 , sizeof(char));
	(*addressBook)[*numContacts - 1].email = (char*)calloc(100 , sizeof(char));
	int choice;
	do
	{
		printf("\n\n####  Add Contact #####\n\n");
		printf("0.Back\n1.Name\t\t: %s\n2.Phone Number\t: %s\n3.email\t\t: %s\n\nPlease select the option: ",(*addressBook)[*numContacts - 1].name,(*addressBook)[*numContacts - 1].phoneNumber,(*addressBook)[*numContacts - 1].email);
		scanf("%d",&choice);
		switch(choice)
		{
			case 0:
				return;
			case 1:
				printf("Enter the Name: ");
				scanf(" %[^\n]s", (*addressBook)[*numContacts - 1].name);
				break;
			case 2:
				printf("Enter the Phonenumber: ");
				scanf(" %[^\n]s", (*addressBook)[*numContacts - 1].phoneNumber);
				break;
			case 3:
				printf("Enter the email: ");
				scanf(" %[^\n]s", (*addressBook)[*numContacts - 1].email);
				break;
		}
	}
	while(choice >= 0 && choice <= 3);
}


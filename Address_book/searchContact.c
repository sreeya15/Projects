#include "address_book.h"

void searchContact(struct Contact** addressBook, int* numContacts)
{
	int choice,t,serial_no;
	char s[100]={'\0'};
	printf("0. Back\n1. Name\n2. Phone Number\n3. Email ID\n4. Serial No\n\nPlease select an option: ");
	scanf("%d",&choice);
	switch(choice)
	{
		case 0:
			return;
			break;
		case 1:
			printf("Enter the Name: ");
			scanf("%s",s);
			for(int i=0;i<*numContacts;i++)
			{
				if(strcmp((*addressBook)[i].name,s) == 0)
				{
					t=i;
					break;
				}
			}
			break;
		case 2:
			printf("Enter the Phone Number: ");
			scanf("%s",s);
			for(int i=0;i<*numContacts;i++)
			{
				if(strcmp((*addressBook)[i].phoneNumber,s) == 0)
				{
					t=i;
					break;
				}
			}
			break;
		case 3:
			printf("Enter the Email Id: ");
			scanf("%s",s);
			for(int i=0;i<*numContacts;i++)
			{
				if(strcmp((*addressBook)[i].email,s) == 0)
				{
					t=i;
					break;
				}
			}
			break;
		case 4:
			printf("Enter the Serial No: ");
			scanf("%d",&serial_no);
			serial_no--;
			if(serial_no < *numContacts)
			{
				t=serial_no;
			}
			break;
	}
	printf("\n#### Searched Contacts ####\n\nS.no\t|Name\t\t\t|Phone Number\t\t|Email id\t\n");
	printf("\n%d\t %s\t\t\t%s\t\t%s\n",t+1,(*addressBook)[t].name,(*addressBook)[t].phoneNumber,(*addressBook)[t].email);
}

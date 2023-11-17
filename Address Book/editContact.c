#include "address_book.h"

void editContact(struct Contact** addressBook, int* numContacts)
{
	printf("\nSelect the serial.No: ");
	int serial_no;
	scanf("%d",&serial_no);
	serial_no--;
	if(*numContacts != 0 && serial_no < *numContacts)
	{
		int option;
		do
		{
			printf("0.Back\n1.Name\t\t: %s\n2.Phone Number\t: %s\n3.email\t\t: %s\n\nPlease select the option: ",(*addressBook)[serial_no].name,(*addressBook)[serial_no].phoneNumber,(*addressBook)[serial_no].email);
			scanf("%d",&option);
			switch(option)
			{
				case 0:
					return ;
					break;
				case 1:
					printf("Enter the New Name: ");
					scanf(" %[^\n]s", (*addressBook)[serial_no].name);
					break;
				case 2:
					printf("Enter the New Phonenumber: ");
					scanf(" %[^\n]s", (*addressBook)[serial_no].phoneNumber);
					break;
				case 3:
					printf("Enter the New email: ");
					scanf(" %[^\n]s", (*addressBook)[serial_no].email);
					break;
			}
		}while(option >=0 && option <= 3);
	}
	else
		printf("\nNo contact found\n");
}

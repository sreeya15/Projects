#include "address_book.h"

void deleteContact(struct Contact** addressBook, int* numContacts)
{
	printf("\nEnter the Serial Number: ");
	int serial_no;
	scanf("%d",&serial_no);
	serial_no--;
	if(serial_no < *numContacts)
	{
		int option;
		do{
			printf("0.Back\n1.Name\t\t: %s\n2.Phone Number\t: %s\n3.email\t\t: %s\n\nPlease select the option: ",(*addressBook)[serial_no].name,(*addressBook)[serial_no].phoneNumber,(*addressBook)[serial_no].email);
			scanf("%d",&option);
			switch(option)
			{
				case 0:
					return ;
					break;
				case 1:
					strcpy((*addressBook)[serial_no].name,"\0");
					break;
				case 2:
					strcpy((*addressBook)[serial_no].phoneNumber,"\0");
					break;
				case 3:
					strcpy((*addressBook)[serial_no].email,"\0");					break;
			}
			if((strncmp((*addressBook)[serial_no].name,"\0",1) == 0 ) && (strncmp((*addressBook)[serial_no].phoneNumber,"\0",1) == 0) && (strncmp((*addressBook)[serial_no].email,"\0",1) == 0))
			{
				for(int i = serial_no;i<*numContacts;i++)
				{
					(*addressBook)[i].name=(*addressBook)[i+1].name;
					(*addressBook)[i].phoneNumber=(*addressBook)[i+1].phoneNumber;
					(*addressBook)[i].email=(*addressBook)[i+1].email;
				}
				(*numContacts)--;
				break;
			}
		}while(option >=0 && option <= 3);
	}
}

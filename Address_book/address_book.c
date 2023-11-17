#include "address_book.h"

void print()
{
	printf("\n\n0. Exit\n1. Add Contact\n2. Search Contact\n3. Edit Contact\n4. Delete Contact\n5. List Contact\n6. Save Contact\n\nPlease select the option: "); 
}
int main() {
	struct Contact* addressBook = NULL;
	int numContacts = 0;
	int choice=0;
	while(choice >= 0 && choice <= 6)
	{
		print();
		scanf("%d", &choice);
		switch (choice) {
			case 0:
				return 0;
				break;
			case 1:
				addContact(&addressBook, &numContacts);
				break;
			case 2:
				searchContact(&addressBook,&numContacts); 
				break;
			case 3:
				editContact(&addressBook, &numContacts); 
				break;
			case 4:
				deleteContact(&addressBook, &numContacts); 
				break;
			case 5:
				listContact(&addressBook, &numContacts);
				break;
			case 6:
				saveContact(&addressBook, &numContacts);
				break;
			default:
				printf("Invalid choice. Please try again.\n");
				break;
		}
	}
	for (int i = 0; i < numContacts; i++) {
		free(addressBook[i].name);
		free(addressBook[i].phoneNumber);
		free(addressBook[i].email);
	}
	free(addressBook);

	return 0;
}

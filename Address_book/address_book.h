#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Contact 
{
	char* name;
	char* phoneNumber;
	char* email;
};

void print(void);
void addContact(struct Contact** addressBook, int* numContacts);
void editContact(struct Contact** addressBook, int* numContacts);
void deleteContact(struct Contact** addressBook, int* numContacts);
void searchContact(struct Contact** addressBook, int* numContacts);
void saveContact(struct Contact** addressBook, int* numContacts);
void listContact(struct Contact** addressBook, int* numContacts);

#endif

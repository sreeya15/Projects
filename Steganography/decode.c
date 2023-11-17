#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>

Status open_decode_files(DecodeInfo *dInfo)
{
    // Stego Image file
    dInfo->fptr_stego_image = fopen(dInfo->stego_image_fname, "r");
    // Do Error handling
    if (dInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open stego image file %s\n", dInfo->stego_image_fname);
	return e_failure;
    }
    dInfo->output_fptr = fopen(dInfo->output_fname, "w");
    // Do Error handling
    if (dInfo->output_fptr == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open stego image file %s\n", dInfo->output_fname);
	return e_failure;
    } 
    return e_success;
}

Status do_decoding(DecodeInfo *dInfo,char *magic_string)
{
    if(open_decode_files(dInfo) == e_success)						// Open files
    {
	printf("Success : File open\n");
	if (decode_size_of_magic_string(dInfo) == e_success) 				// Decode the size of the magic string.
	{ 
	    printf("Success : Size of magic string is Decoded\n"); 			// Print a success message indicating that the size of the magic string was successfully decoded.
	    if (decode_magic_string(dInfo, magic_string) == e_success) 			// Decode the magic string.
	    { 
		printf("Success : Magic string is Decoded\n"); 					// Print a message indicating that the magic string was successfully decoded.
		if (decode_secret_file_extn_size(dInfo) == e_success) 
		{ 									// Decode the size of the secret file extension.
		    printf("Success : Secret file extension size Decoded\n"); 			// Print a message indicating that the size of the secret file extension was successfully decoded.
		    if (decode_secret_file_extn(dInfo) == e_success) 
		    { 									// Decode the secret file extension.
			printf("Success : Secret file extension Decoded\n"); 			// Print a message indicating that the secret file extension was successfully decoded.
			if (decode_secret_file_size(dInfo) == e_success) 
			{ 								// Decode the size of the secret file.
			    printf("Success : Secret file size Decoded\n"); 			// Print a message indicating that the size of the secret file was successfully decoded.
			    if (decode_secret_file_data(dInfo) == e_success) 
			    { 								// Decode the data of the secret file.
				printf("Success : Secret file data Decoded\n"); 		// Print a message indicating that the data of the secret file was successfully decoded.
			    }
			    else 
			    {
				printf("Error: Secret file data decoding failed\n"); 	// Print an error message indicating that the decoding of the secret file data failed.
			    }
			} 
			else 
			    printf("Error: Secret file size decoding failed\n"); 	// Print an error message indicating that the decoding of the secret file size failed.
		    } 
		    else 
			printf("Error: Secret file extension decoding failed\n"); 	// Print an error message indicating that the decoding of the secret file extension failed.
		} 
		else 
		    printf("Error: Secret file extension size decoding failed\n"); 	// Print an error message indicating that the decoding of the secret file extension size failed.
	    } 
	    else 
		printf("Error: Magic string decoding failed\n"); 			// Print an error message indicating that the decoding of the magic string failed.
	} 
	else 
	    printf("Error: Size of magic string decoding failed\n"); 			// Print an error message indicating that the decoding of the size of the magic string failed.
    }
    else 
	printf("Error: Couldn't open file\n"); 						// Print an error message indicating that the files could not be opened.
}


Status decode_size_of_magic_string(DecodeInfo *dInfo)						// Magic string size
{
    char image_buff[32];									// Create a buffer to hold the LSBs of the stego image data.
    int temp,num = 0;

    fseek(dInfo->fptr_stego_image, 54, SEEK_SET);
    fread(image_buff, 1, 32, dInfo->fptr_stego_image);						// Read 8 bytes from the stego image file into the buffer.

    for(int i=0;i< 32;i++)
    {
	temp = image_buff[i] & 1;								// Get the LSB of each byte in the buffer.
	num = (temp<<i) | num;									// Shift the LSB to the correct position and OR it with num.
    }

    dInfo->length = num;
    //printf("Magic string size = %d\n",dInfo->length);   
    return e_success;										// Return the status indicating the success of the decoding process.
}

Status decode_magic_string(DecodeInfo *dInfo,char *magic_string)				// Encode the magic string
{
    //printf("Magic string = ");
    char image_buff[8], string[10];

    for (int i = 0; i < dInfo->length; i++)
    {
	int num = 0,temp;
	fread(image_buff, 1, 8, dInfo->fptr_stego_image);
	for (int j = 0; j < 8; j++)
	{
	    temp = image_buff[j] & 1;
	    num = (temp<<j) | num;
	}
	char ch = (char)num;
	//printf("%c",ch);
	string[i] = ch;
    }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *dInfo)						// Secret file extension size
{
    char image_buff[32];
    int temp,num = 0;
    fseek(dInfo->fptr_stego_image, 102, SEEK_SET);

    fread(image_buff, 1, 32, dInfo->fptr_stego_image);

    for(int i=0;i< 32;i++)
    {
	temp = image_buff[i] & 1;
	num = (temp<<i) | num;
    }
    dInfo->extension_size = num;
    //printf("Magic string extension size = %d\n",dInfo->extension_size);   
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *dInfo)						// Secert file extension 
{
    //printf("Secret file extension = ");
    char image_buff[8], string[10];

    for (int i = 0; i < dInfo->extension_size; i++)
    {
	int num = 0,temp;
	fread(image_buff, 1, 8, dInfo->fptr_stego_image);
	for (int j = 0; j < 8; j++)
	{
	    temp = image_buff[j] & 1;
	    num = (temp<<j) | num;
	}
	char ch = (char)num;
	//printf("%c",ch);
	string[i] = ch;
    } 
    return e_success;
}


Status decode_secret_file_size(DecodeInfo *dInfo)					// Secret file size
{
    char image_buff[32];
    int temp,num = 0;
    fread(image_buff, 1, 32, dInfo->fptr_stego_image);

    for(int i=0;i< 32;i++)
    {
	temp = image_buff[i] & 1;
	num = (temp<<i) | num;
    }
    dInfo->size_secret_file = num;
    //printf("Secret file size = %ld\n",dInfo->size_secret_file);   
    return e_success;
}


Status decode_secret_file_data(DecodeInfo *dInfo)					// Secret file data
{
    //printf("Secret file data = ");
    char image_buff[8];									// Create a buffer to hold the LSBs of the stego image data.
    //printf("SECRET FILE SIZE = %ld\n",dInfo->size_secret_file);
    for (int i = 0; i < dInfo->size_secret_file; i++)
    {
	int num = 0,temp;
	fread(image_buff, 1, 8, dInfo->fptr_stego_image);				// Read 8 bytes from the stego image file into the buffer.
	for (int j = 0; j < 8; j++)
	{
	    temp = image_buff[j] & 1;							// Get the LSB of each byte in the buffer.
	    num = (temp<<j) | num;							// Shift the LSB to the correct position and OR it with num.
	}
	char ch = (char)num;
	//printf("%c ",ch);
	fwrite(&ch, 1, 1, dInfo->output_fptr);						// Write the character to the output file.
    }
    //printf("success\n");
    return e_success;									// Return the status indicating the success of the decoding process.
}












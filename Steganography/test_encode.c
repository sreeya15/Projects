#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

#define MAX_MAGIC_STRING_LENGTH 10 

unsigned char magic_string[MAX_MAGIC_STRING_LENGTH];                                 	// Global variable (magic string declaration)

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;								    	// Structure variables declaration 
    DecodeInfo dInfo;
    uint img_size;

    int var; 
    encInfo.secret_fname = "secret.txt";					   	// Secret file name

    if(argc >= 3)									// Check If command line argument count is greater than or equal to 3
    {
	var = check_operation_type(argv);						// Function call (check the operation type; if its encoding)
	if(var == e_encode)
	{
	    printf("Chosen Encoding\n");
	    if(read_and_validate_encode_args(argv,&encInfo) == e_success)		// Reading and validating the command line arguments; check the return value
	    {
		printf("Success : Read and Validate function\n");
		if(do_encoding(&encInfo,magic_string) == e_success)			// Function call for encoding operation
		{
		    printf("Success: open file function\n");				// Print success message if the return value is e_success.
		}
	    }
	    else
	    {
		printf("Error Encoding\n");						// Else print error message
	    }
	}
	else if(var == e_decode)							// Function call (check the operation type; if its decoding)
	{
	    printf("Choosen Decoding\n");
	    if(read_and_validate_decode_args(argv,&dInfo) == e_success)			// Reading and validating the command line arguments; check the return value
	    {
		printf("Success : Read and Validate function\n");
		if(do_decoding(&dInfo,magic_string) == e_success)			// Function call for Decoding operation
		{
		    printf("Success: open file function\n");				// Print success message if the return value is e_success.
		}
	    }
	    else
	    {
		printf("Error Decoding\n");						// Else print error message
	    }
	}
    }
    else
    {
	printf("Error Insufficient number of command line argument\n");			// Print error message if the command line arguments are insufficient.
    }

    return 0;
}

OperationType check_operation_type(char *argv[])					// Function definition
{
    if(strcmp(argv[1], "-e") == 0)							// Check if the operation is encoding
    {
	return e_encode;								// Return respective enum if true
    }
    else if(strcmp(argv[1], "-d") == 0)							// Check if the operation is decoding
    {
	return e_decode;								// Return respective enum if true
    }
    else
    {
	return e_unsupported;								// Return respective enum if false
    }
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)			// Read and validate the command line arguments for encoding
{
    if(strcmp((strstr(argv[2],".")),".bmp")==0)						// strstr finds the sub-string "." ; strcmp compares the string ".bmp" with the returned string
    {
	encInfo->src_image_fname = argv[2] ;						// Stores the source image file name
	//printf("Image file check Done\n");
    }
    else
    {
	printf("Enter proper file name with .bmp extension\n");				// Prints error if condition becomes false
	return e_failure;
    }

    if(strcmp((strstr(argv[3],".")),".txt")==0)						// strstr finds the sub-string "." ; strcmp compares the string ".txt" with the returned string
    {
	encInfo->secret_fname,argv[3] = argv[3];					// Stores the secret image file name
	//printf("Scret text File checked\n");
    }
    else
    {
	printf("Enter proper file name with .txt extension\n");				// Prints error if condition becomes false
	return e_failure;
    }
    if(argv[4] == NULL)									// If source image is not provided then create one 
    {
	encInfo->stego_image_fname = "stego_image.bmp";					// Stores the image file name
	//printf("File created\n");
    }
    else
    {
	encInfo->stego_image_fname = argv[4];						// Stores the image file name if provided
    }
    return e_success;									// Return e_success enum 
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *dInfo)			// Read and validate the command line arguments for decoding
{
    if(strcmp((strstr(argv[2],".")),".bmp")==0)						// strstr finds the sub-string "." ; strcmp compares the string ".bmp" with the returned string
    {
	dInfo->stego_image_fname = argv[2] ;						// Store the source file name
	//printf("Image file check Done\n");
    }
    else
    {
	printf("Enter proper file name with .bmp extension\n");				// Prints error if condition becomes false
	return e_failure;
    }

    if(argv[3] == NULL)									// If file name is not provided then create one 
    {
	dInfo->output_fname = "output.txt";						// Store ouput file name
	//printf("File created\n");
    }
    else
    {
	dInfo->output_fname = argv[3];							// Stores the file name if provided
    }
    return e_success;									// Return e_success enum 
}

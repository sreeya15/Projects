#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;												// Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);										// Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);										// Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);										// Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");						// Open Src Image file in read mode
    if (encInfo->fptr_src_image == NULL)									// Do Error handling; check if the file is opened properly
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open src image file %s\n", encInfo->src_image_fname);			// Print error if your not able to open the file
	return e_failure;											// Return e_failure enum
    }
    
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");							// Open Secret file in read mode
    if (encInfo->fptr_secret == NULL)										// Do Error handling; check if the file is opened properly
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open secret file%s\n", encInfo->secret_fname);			// Print error if your not able to open the file
	return e_failure;											// Return e_failure enum
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");						// Open Stego Image file in write mode
    if (encInfo->fptr_stego_image == NULL)									// Do Error handling; check if the file is opened properly
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open stego image file %s\n", encInfo->stego_image_fname);		// Print error if your not able to open the file
	return e_failure;											// Return e_failure enum
    }
    // No failure return e_success
    return e_success;												// Return e_success enum
}

Status do_encoding(EncodeInfo *encInfo,char *magic_string)							// Encoding function definition
{
    if(open_files(encInfo) == e_success)									// Function call to open files and check if the return value is e_success
    {
	printf("Success : Open files\n");
	if(check_capacity(encInfo) == e_success)								// Function call to check capacity and check if the return value is e_success
	{
	    printf("Success : Check capacity\n");
	    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)		// Function call to copy header and check if the return value is e_success
	    {
		printf("Success : Copy bmp header\n");
		if(read_magic_string(magic_string) == e_success)						// Function call to Read magic string from the user
		{
		    encInfo->length = calculate_magic_string_length(magic_string);				// Function call to calculate the length of magic string
		    //printf("Magic string length: %d\n", encInfo->length);
		    //printf("\n");
		    if(encode_size_of_magic_string(encInfo) == e_success)					// Function call to encode magic string size
		    {
			//printf("\nSize_of_magic_string_function_done\n");
			if(encode_magic_string(magic_string,encInfo) == e_success)				// Function call to encode magic string
			{
			    printf("Success : Encoded Magic String\n");
			    //printf("\n");

			    encInfo-> extension_size = secret_file_extension_size(encInfo->secret_fname, encInfo);		// Function call to secret file extension size
			    printf("Extension size = %d\n",encInfo->extension_size);

			    if(encode_secret_file_extension_size(encInfo->extension_size, encInfo) == e_success)		// Function call to encode secret file extension size
			    {
				printf("Success : Encoded Secret file extension size\n");
				if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)			// Function call to encode secret file extension 
				{
				    printf("Success : Encoded Secret file extension\n");
				    if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)			// Function call to encode secret file size
				    {
					printf("Success : Encoded Secret file size\n");
					if(encode_secret_file_data(encInfo) == e_success)					// Function call to encode secret file data
					{
					    printf("Success : Encoded Secret file data\n");
					    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)	// Function call to copy remaining image data
						printf("Success : Remaining data is copied\n");
					    else
						printf("Error din't copy Remaining data\n");					// Printing the respective error messages
					}
					else	
					    printf("Error Secret file data is not encoded\n");
				    }
				    else
				    {
					printf("Error secret file size is not encoded\n");
				    }
				}
				else
				    printf("Error Secret file extension is not encoded\n");
			    }
			    else
				printf("Error Secret file extension size is not encoded\n");
			}
			else
			    printf("Error Magic string is not encoded\n"); 
		    }
		    else
			printf("Error size of magic string is not encoded\n");
		}
		else
		    printf("Error could'nt read magic string\n");
	    }
	    else
		printf("Error could'nt copy BMP header\n");
	}
	else
	    printf("Error Could'nt check capacity\n");
    }
    else
	printf("Error Could'nt open file\n");
}

Status check_capacity(EncodeInfo *encInfo)									// Function definition for Check capacity
{   
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);					// Function call get the image size
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);						// Function call to get Secret file size
    printf("Secret file size = %ld\n",encInfo->size_secret_file);
    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)						// Function definition for Copy BMP header
{
    char *header[BMP_HEADER];											// Buffer for Bmp header[54 bytes]

    fseek(fptr_src_image,0,SEEK_SET);										// Seek the file pointer position to the beginning of the file
    fread(header,sizeof(char),BMP_HEADER,fptr_src_image);							// Reads a block of memory from the given input file stream
    fwrite(header,sizeof(char),BMP_HEADER,fptr_dest_image);							// Writes to the output file stream
    return e_success;												// Return e_success enum
}

uint get_file_size(FILE *fptr)											// Function definition for File size
{
    fseek(fptr,0,SEEK_END);											// Seek the file pointer position to the end of file.
    return ftell(fptr);												// Find out the position of file pointer in the file and return it
}

Status read_magic_string(char* magic_string)									// Function definition for Reading the magic string from the user
{
    printf("Enter the magic string : ");
    scanf("%s", magic_string);
    printf("Magic string : %s\n", magic_string);								// Print the magic string
    return e_success;												// Return e_success enum
}

int calculate_magic_string_length(const char* magic_string)							// Function definition for Calculating the length of magic string
{
    return strlen(magic_string);										// Return the length of the magic string
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)					// Function definition for Encoding the magic string
{
    char buff[8];												// Buffer for magic string
    int i;
    for(i = 0; i < encInfo->length; i++)									// Loop until length of magic string
    {
	fread(buff,1,8,encInfo->fptr_src_image);								// Reads a block of memory from the given input file stream
	encode_byte_to_lsb(magic_string[i],buff);								// Function call to encode byte to lsb
	fwrite(buff,1,8,encInfo->fptr_stego_image);								// Writes to the output file stream
    }
    return e_success;												// Return e_success enum
}

Status encode_byte_to_lsb(char data, char *image_buffer)							// Function defintion for Encoding byte to LSB
{
    int temp, num=0;												// Initialize variables to hold temporary values and the final number
    for(int i = 0; i < 8; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);								// Clear the LSB of the image_buffer[i] by bitwise ANDing with the complement of 1.
	temp = data & 1 ;											// Get the LSB of the data.
	image_buffer[i] = image_buffer[i] | temp;								// Set the LSB of image_buffer[i] to the LSB of the data by bitwise ORing with temp.
	data = data >> 1;											// Shift the data one bit to the right.
    }
    for(int i=0;i<8;i++)											// Reconstruct the number from the LSBs in the image buffer.
    {
	temp= image_buffer[i] & 1;										// Get the LSB of image_buffer[i].
	num = (temp<<i) | num;											// Shift the LSB to the correct position and OR it with num.
    }
    char ch = (char)num;											// Converting reconstructed number to a character.
    //printf("%c ",ch);  												// Print the character.  
    return e_success;												// Return the status indicating the success of the encoding process.
}


Status encode_size_of_magic_string(EncodeInfo *encInfo)								// Encode magic string size 32bytes
{
    //printf("Encode Magic string size\n");
    char buff[32];												// Create a buffer to hold the magic string size (32 bytes).

    fread(buff,1,32,encInfo->fptr_src_image);									// Read 32 bytes from the source image file into the buffer.
    encode_size_byte_to_lsb(encInfo->length,buff);								// Function call to Encode the size byte into the LSBs of the buffer.
    fwrite(buff,1,32,encInfo->fptr_stego_image);								// Write the modified buffer containing the encoded size into the stego image file.
    
    return e_success;												// Return the status indicating the success of the encoding process.
}

Status encode_size_byte_to_lsb(int data, char *image_buffer)							// Function definition for Encode magic string size to LSB
{
    int temp, num=0;												// Initialize variables to hold temporary values and the final number
    for(int i = 0; i < 32; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);								// Clear the LSB of the image_buffer[i] by bitwise ANDing with the complement of 1.
	temp = data & 1 ;											// Get the LSB of the data.
	image_buffer[i] = image_buffer[i] | temp;								// Set the LSB of image_buffer[i] to the LSB of the data by bitwise ORing with temp.
	data = data >> 1;											// Shift the data one bit to the right.
    }
    return e_success;												// Return the status indicating the success of the encoding process.
}

int secret_file_extension_size(const char *filename, EncodeInfo *encInfo)					// Calculate the Secret file extension size
{
    const char *extension = strrchr(filename,'.');								// Find the secret file extension by locating the last occurrence of '.' in the filename								      					// Find the Secret file extension 														
    strcpy(encInfo->extn_secret_file,extension);		 						// Copy the extension to the extn_secret_file field in the EncodeInfo structure

    if(encInfo->extn_secret_file!= NULL)
	printf("Extension = %s\n",encInfo->extn_secret_file);							// Print the extension if it is found.
    else
	printf("Match not found\n");										// Print a message indicating that the extension was not found.
    return strlen(encInfo->extn_secret_file);									// Return the size of the secret file extension.
}

Status encode_secret_file_extension_size(int extension_size, EncodeInfo *encInfo)				// Function definition for Encode Secret file extension size
{
    char buff[32];
    fread(buff,1,32,encInfo->fptr_src_image);
    encode_extension_size_byte_to_lsb(extension_size,buff);
    fwrite(buff,1,32,encInfo->fptr_stego_image);
    return e_success;
}


Status encode_extension_size_byte_to_lsb(int data, char *image_buffer)						// Function definition for Encode the secret file extension to LSB
{
    int temp, num=0;
    for(int i = 0; i < 32; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = data & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	data = data >> 1;
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)					// Function definition for Encode Secret file extension
{
    char buff[8];
    int i;
    for(i = 0; i < encInfo->extension_size; i++)
    {
	fread(buff,1,8,encInfo->fptr_src_image);
	encode_secret_file_extn_byte_to_lsb(encInfo->extn_secret_file[i],buff);
	fwrite(buff,1,8,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_secret_file_extn_byte_to_lsb(char data, char *image_buffer)				// Function definition for Encode Secret file extension to LSB
{
    int temp, num=0;
    for(int i = 0; i < 8; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = data & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	data = data >> 1;
    }										
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)					// Function definition for Encode Secret file size
{
    char size_buff[32];

    fread(size_buff,1,32,encInfo->fptr_src_image);
    encode_secretfile_size_to_lsb(file_size, size_buff);
    fwrite(size_buff,1,32,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secretfile_size_to_lsb(long file_size, char *image_buffer)				// Function definition for Encode Secret file size to LSB
{
    int temp, num=0;
    for(int i = 0; i < 32; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = file_size & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	file_size = file_size >> 1;
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)							// Function definition for Encode secret file data
{
    char buff[encInfo->size_secret_file];								// Create a buffer to hold the data of the secret file.
    fseek(encInfo->fptr_secret,0,SEEK_SET);								// Move the file pointer of the secret file to the beginning.
    for(int i=0;i<encInfo->size_secret_file;i++)
    {
	char ch = fgetc(encInfo->fptr_secret);								// Read a character from the secret file.
	fread(buff, 1,8, encInfo->fptr_src_image);							// Read 8 bytes from the source image file into the buffer.
	encode_byte_to_lsb(ch,buff);									// Encode the character into the LSBs of the buffer.
	fwrite(buff,1,8,encInfo->fptr_stego_image);							// Write the modified buffer containing the encoded data into the stego image file.
    }
    return e_success;
}

Status encode_data_byte_to_lsb(char data, char *image_buffer, int size)					// Function definition for Encode the data to LSB
{
    int temp, num=0;
    for(int i = 0; i < size; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = data & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	data = data >> 1;
    }
    return e_success;				
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)						// Function definition for Copying the remaining data
{
    int ch;												// Variable to hold the character read from the source file.    													
    while ((ch = fgetc(fptr_src)) != EOF)								// Read each character from the source file until the end of file (EOF) is reached.										
    {
	fputc(ch, fptr_dest); 										// Write the character to the destination file.										
    }
    return e_success; 											// Return the status indicating the success of the copying process.											
}





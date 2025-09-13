#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include<stdlib.h>




Status do_decoding(DecodeInfo *decInfo)
{
    printf("\n## DECODING OPERATION STARTED ##\n\n");
    //  Open stego image file
    if (open_image_file(decInfo) != e_success)
    {
        printf("❌Error: Failed to open stego image file\n");
        return e_failure;
    }
    printf("Image file opened successfully\n");

    // Decode magic string
    if (decode_magic_string(decInfo->magic_string, decInfo) != e_success)
    {
        printf("❌Error: Magic string decoding failed\n");
        return e_failure;
    }
    printf("Magic string decoded successfully\n");

    //  Decode secret file extension size
    if (decode_secret_file_extn_size(&(decInfo->size_secret_file_extn), decInfo) != e_success)
    {
        printf("❌ Error: Failed to decode secret file extension size\n");
        return e_failure;
    }
    printf("Secret file extension size decoded successfully\n");

    // Decode secret file extension
    if (decode_secret_file_extn(decInfo) != e_success)
    {
        printf("❌Error: Failed to decode secret file extension\n");
        return e_failure;
    }
    printf("Secret file extension decoded successfully\n");

    // Decode secret file size
    if (decode_secret_file_size(decInfo) != e_success)
    {
        printf("❌Error: Failed to decode secret file size\n");
        return e_failure;
    }
    printf("Secret file size decoded successfully\n");

    // Decode secret file data
    if (decode_secret_file_data(decInfo) != e_success)
    {
        printf("❌Error: Failed to decode secret file data\n");
        return e_failure;
    }
    printf("Secret file data decoded successfully\n");

    return e_success;
}

Status open_image_file(DecodeInfo *decInfo)
{
    printf("File Opening required\n");
    // Try to open the stego image file in read mode
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

    // Check if the file was opened successfully
    if (decInfo->fptr_stego_image == NULL)
    {
       
        printf("❌Error: Failed to open the image file: %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    else
    {
        printf("Opened image file : %s\n", decInfo->stego_image_fname);
    }

    return e_success;
}

// Closing files
void close_decode_files(DecodeInfo *decInfo)
{
    printf("Closing all opened files\n");
    int any_file_closed = 0;

    // Close stego image file
    if (decInfo->fptr_stego_image != NULL)
    {
        if (fclose(decInfo->fptr_stego_image) == 0)
        {
            printf("Closed image file: %s\n", decInfo->stego_image_fname);
            decInfo->fptr_stego_image = NULL;
            any_file_closed = 1;
        }
        else
        {
            printf("❌Failed to close image file : %s\n", decInfo->stego_image_fname);
        }
    }

    // Close secret output file
    if (decInfo->fptr_secret != NULL)
    {
        if (fclose(decInfo->fptr_secret) == 0)
        {
            printf("Closed secret output file : %s\n", decInfo->secret_fname);
            decInfo->fptr_secret = NULL;
            any_file_closed = 1;
        }
        else
        {
            printf("❌Failed to close secret output file: %s\n", decInfo->secret_fname);
        }
    }

    if (any_file_closed)
    {
        printf("Opened Files closed successfully\n");
    }
    else
    {
        printf("No decode files were open to close\n");
    }
}

// Function to read and validate decode arguments from argv
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Initialize all file pointers to NULL to prevent undefined behavior
    decInfo->fptr_stego_image = NULL;
    decInfo->fptr_secret = NULL;

    printf("Validating decoding arguments...\n");

    // Validate stego image file extension
    char *bmp_ext = strstr(argv[2], ".bmp");
    if (bmp_ext != NULL && strcmp(bmp_ext, ".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
        printf("Source image validated: %s\n", argv[2]);
    }
    else
    {
        printf("❌Error: Invalid stego image file. Expected a .bmp file\n");
        return e_failure;
    }
    
    if (argv[3] == NULL)
    {
        // User did not provide output filename, use default
        decInfo->secret_fname = "decode_secret";
    }
    else
    {
        char temp_name[100];  // Adjust size as needed
        strncpy(temp_name, argv[3], sizeof(temp_name) - 1);
        temp_name[sizeof(temp_name) - 1] = '\0';

        char *ext = strrchr(temp_name, '.');
        if (ext != NULL)
        {
            *ext = '\0';  // Truncate extension
            printf("Note: Ignoring file extension in output filename.\n");
        }
        decInfo->secret_fname = strdup(temp_name);  // Store in heap

    }

    printf("All decode arguments validated successfully!\n");
    return e_success;
}

/* Decode and validate the magic string embedded in the image */
Status decode_magic_string(char *expected_magic_string, DecodeInfo *decInfo)
{
    printf("Decoding Magic String from image...\n");

    // Move the file pointer to the start of pixel data (usually at byte 54 in BMP)
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    // Assume expected magic string is constant and valid
    int magic_len = strlen(expected_magic_string);

    // Buffer to store decoded magic string
    char decoded_magic[magic_len + 1];

    // Decode 'magic_len' characters (magic_len * 8 bits)
    if (decode_data_from_image(decoded_magic, magic_len, decInfo->fptr_stego_image, decInfo) != e_success)
    {
        printf("❌Error: Failed to decode magic string data from image\n");
        return e_failure;
    }

    // Null-terminate manually
    decoded_magic[magic_len] = '\0';

    // Compare decoded vs expected magic string
    if (strcmp(expected_magic_string, decoded_magic) != 0)
    {
        printf("❌Error: Magic string not matching. Expected \"%s\", found \"%s\"\n", expected_magic_string, decoded_magic);
        return e_failure;
    }

    return e_success;
}

/*decode the secret file extension size */
Status decode_secret_file_extn_size(int *file_size, DecodeInfo *decInfo)
{
    printf("Decoding secret file extension size from image\n");

    char buffer[32];

    // Read 32 bytes from the source image
    if (fread(buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        printf("❌Error: Unable to read from source image while encoding file size\n");
        return e_failure;
    }

    // Encode the 32-bit file size into LSBs
    if (decode_int_to_lsb(file_size, buffer) != e_success)
    {
        printf("❌Error: Failed to encode file size into image buffer\n");
        return e_failure;
    }

    // Print success message
    return e_success;
}

/* Decode the secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    printf("Decoding secret file extension from image\n");

    // Allocate a buffer with space for null terminator
    char extn[decInfo->size_secret_file_extn + 1];

    // Decode the file extension
    if (decode_data_from_image(extn, decInfo->size_secret_file_extn, decInfo->fptr_stego_image, decInfo) != e_success)
        return e_failure;

    extn[decInfo->size_secret_file_extn] = '\0'; // Null-terminate the decoded extension

    strcpy(decInfo->extn_secret_file, extn); // Store it in DecodeInfo

    int f = 1;
    if (strcmp(decInfo->secret_fname, "decode_secret") == 0)
    {
        f = 0;
        printf("Output file not mentioned. Creating decode_secret%s as default...\n", extn);
    }

    // Construct final secret file name with extension
    char output_filename[100];
    // snprintf() is cleaner and safer than strcpy() + strcat()
    // Safe from Overflow	Null-Terminates	Recommended
    snprintf(output_filename, sizeof(output_filename), "%s%s", decInfo->secret_fname, extn);
    //"%s%s": Format string meaning "concatenate two strings".
    decInfo->secret_fname = strdup(output_filename); // Dynamically allocate the final filename
    // strdup() allocates new memory for the final filename

    if (f)
    {
        printf("Output file mentioned. Created your %s file\n", decInfo->secret_fname);
    }
    // printf("Secret file extension decoded and output filename set\n");
    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    printf("Decoding secret file size from image...\n");

    char buffer[32];

    // Read 32 bytes from the stego image
    if (fread(buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        printf("Failed to read data for decoding file size\n");
        return e_failure;
    }

    // printf("%d\n", decInfo->decode_size_secret_file);

    // Decode the file size from LSBs
    if (decode_int_to_lsb(&(decInfo->decode_size_secret_file), buffer) != e_success)
    {
        printf("❌Failed to decode file size from image buffer\n");
        return e_failure;
    }

    // printf("%d\n", decInfo->decode_size_secret_file);
    //  printf("File size decoded successfully\n");
    return e_success;
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("Decoding secret file data from image...\n");

    // Dynamically allocate buffer to store the secret file data
    char *data = (char *)malloc(decInfo->decode_size_secret_file + 1); // +1 for null-terminator
    if (data == NULL)
    {
        printf("Memory allocation failed\n");
        return e_failure;
    }

    // Decode the secret file data from the stego image
    if (decode_data_from_image(data, decInfo->decode_size_secret_file, decInfo->fptr_stego_image, decInfo) != e_success)
    {
        free(data);
        return e_failure;
    }

    // Open the secret file for writing
    printf("Opening output %s file\n", decInfo->secret_fname);
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w+");
    if (decInfo->fptr_secret == NULL)
    {
        printf("❌Failed to open secret output file for writing : %s\n", decInfo->secret_fname);
        free(data);
        return e_failure;
    }
    else
    {
        printf("Opened secret output file for writing : %s\n", decInfo->secret_fname);
    }

    // Write the decoded data into the secret file
    printf("Writing decoded data to output file: %s\n", decInfo->secret_fname);
    fprintf(decInfo->fptr_secret, "%s", data);

    // Free dynamically allocated memory
    free(data);

    return e_success;
}

/* Decode data from stego image to buffer */
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    char buffer[8];

    for (int i = 0; i < size; i++)
    {
        // Read 8 bytes from the stego image
        if (fread(buffer, 8, 1, fptr_stego_image) != 1)
        {
            printf("❌Failed to read from stego image\n");
            return e_failure;
        }

        // Decode 1 byte from 8 LSBs
        if (decode_lsb_to_byte(&data[i], buffer) != e_success)
        {
            printf("❌Failed to decode byte\n");
            return e_failure;
        }
    }

    return e_success;
}

/* Decode a byte from the LSBs of 8 image bytes */
Status decode_lsb_to_byte(char *data, char *image_buffer)
{
    char ch = 0;
    int i;

    for (i = 0; i < 8; i++)
    {
        ch = ch << 1 | (image_buffer[i] & 1); // Extract LSB and place it at correct bit position
    }

    *data = ch; // Store decoded byte

    return e_success;
}

/*Decode secret file extension size*/
Status decode_int_to_lsb(int *data, char *buffer)
{
    int value = 0;
    int i;

    // Extract 32 bits from the LSBs of the buffer
    for (i = 0; i < 32; i++)
    {
        value = value << 1;       // Shift left to make room for next bit
        value |= (buffer[i] & 1); // Extract LSB from current byte and set it
    }

    *data = value; // Store the decoded integer value

    return e_success;
}
Status display_decoded_message(DecodeInfo *decInfo)
{
    char choice, ch;

    printf("Do you want to view the decoded message?\n");

    while (1)
    {
        printf("Press (y/n): ");
        if (scanf(" %c", &choice) != 1)
        {
            while (getchar() != '\n')
                ; // Clear input buffer
            printf("Invalid input. Please enter 'y' or 'n'.\n");
            continue;
        }

        choice = tolower(choice);

        if (choice == 'y')
        {
            decInfo->fptr_secret = fopen(decInfo->secret_fname, "r");
            rewind(decInfo->fptr_secret);

            printf("Decoded message :\n");
            while (fread(&ch, 1, 1, decInfo->fptr_secret) == 1)
            {
                putchar(ch);
            }
            printf("\n");
            fclose(decInfo->fptr_secret);
            printf("Closed secret file : %s\n", decInfo->secret_fname);
            return e_success;
        }
        else if (choice == 'n')
        {
            printf("Message not displayed as per your choice\n");
            return e_success;
        }
        else
        {
            printf("Invalid option. Please enter 'y' or 'n'.\n");
           // while (getchar() != '\n')
                ;
        }
    }

    return e_failure; // Fallback return, though logically unreachable
}


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "encode.h"
#include "types.h"

Status open_files(EncodeInfo *encInfo)
{
    printf("Opening required files...\n");
    // Open source image file for reading
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if (encInfo->fptr_src_image == NULL)
    {
        printf("Error: Unable to open source image file: %s\n", encInfo->src_image_fname);
        return e_failure;
    }
    else
    {
        printf("Opened source image file : %s\n", encInfo->src_image_fname);
    }

    // Open secret file for reading
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL)
    {
        printf("Error: Unable to open secret file: %s\n", encInfo->secret_fname);
        return e_failure;
    }
    else
    {
        printf("Opened secret file: %s\n", encInfo->secret_fname);
    }

    // Open stego image file for writing
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    if (encInfo->fptr_stego_image == NULL)
    {
        printf("Error: Unable to open stego image file : %s\n", encInfo->stego_image_fname);
        return e_failure;
    }
    else
    {
        printf("Opened stego image file for writing : %s\n", encInfo->stego_image_fname);
    }

    // All files opened successfully
    return e_success;
}

void close_encode_files(EncodeInfo *encInfo)
{
    printf("Closing all opened files...\n");
    int any_file_closed = 0;

    // Close source image file
    if (encInfo->fptr_src_image != NULL)
    {
        if (fclose(encInfo->fptr_src_image) == 0)
        {
            printf("Closed source image file : %s\n", encInfo->src_image_fname);
            encInfo->fptr_src_image = NULL;
            any_file_closed = 1;
        }
        else
        {
            printf("Failed to close source image file : %s\n", encInfo->src_image_fname);
        }
    }

    // Close secret file
    if (encInfo->fptr_secret != NULL)
    {
        if (fclose(encInfo->fptr_secret) == 0)
        {
            printf("Closed secret file : %s\n", encInfo->secret_fname);
            encInfo->fptr_secret = NULL;
            any_file_closed = 1;
        }
        else
        {
            printf("Failed to close secret file : %s\n", encInfo->secret_fname);
        }
    }

    // Close stego image file
    if (encInfo->fptr_stego_image != NULL)
    {
        if (fclose(encInfo->fptr_stego_image) == 0)
        {
            printf("Closed output image file : %s\n", encInfo->stego_image_fname);
            encInfo->fptr_stego_image = NULL;
            any_file_closed = 1;
        }
        else
        {
            printf("Failed to close output image file : %s\n", encInfo->stego_image_fname);
        }
    }

    if (any_file_closed)
    {
        printf("Files closed successfully\n");
    }
}


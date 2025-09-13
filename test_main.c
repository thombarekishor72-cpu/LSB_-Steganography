/*
    Name:Prashant C.P.
    roll no.:25008_025
*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if (argc < 2)
    {
        fprintf(stderr, "   To encode : %s -e <input.bmp> <secret.txt|.c|.sh> [optional output.bmp]\n", argv[0]);
        fprintf(stderr, "   To decode : %s -d <stego_image.bmp> [optional output_file]\n", argv[0]);
        return 0;
    }
    if(check_operation_type(argv)==e_encode)
    {
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            char input[10];

            printf("Enter Magic string : ");
            scanf(" %s",input);

            encInfo.magic_size=strlen(input);
            
            // Manually remove newline character if present
            for (int i = 0; input[i] != '\0'; i++) {
                if (input[i] == '\n') {
                    input[i] = '\0';
                    break;
                }
            }

            // Allocate memory and copy string
            encInfo.magic_string = malloc(strlen(input) + 1);
            if (encInfo.magic_string== NULL) 
            {
                printf("Memory allocation failed.\n");
                return 1;
            }

            strcpy(encInfo.magic_string, input);
                           
             
            
            if(do_encoding(encInfo)==e_success)
            {
                close_encode_files(&encInfo);
                printf("\nHidden data encoded into successfully!\n");
                
            }
            
            free(encInfo.magic_string);
            
        }
        
    }
    else if(check_operation_type(argv)==e_decode)
    {
    
       if (read_and_validate_decode_args(argv,&decInfo) == e_success)
        {
            char input[10];

            printf("Enter Magic string : ");
            scanf(" %s",input);

            decInfo.magic_size=strlen(input);
            
            // Manually remove newline character if present
            for (int i = 0; input[i] != '\0'; i++) {
                if (input[i] == '\n') {
                    input[i] = '\0';
                    break;
                }
            }

            // Allocate memory and copy string
            decInfo.magic_string = malloc(strlen(input) + 1);
            if (decInfo.magic_string== NULL) 
            {
                printf("Memory allocation failed.\n");
                return 1;
            }

            strcpy(decInfo.magic_string, input);
            //printf("%s\n",decInfo.magic_string);         
             
            
            if(do_decoding(&decInfo)==e_success)
            {
                close_decode_files(&decInfo);
                printf("\nHidden data encoded into successfully!\n");
                
            }
            
            free(decInfo.magic_string);
    }
    else if(check_operation_type(argv))
    {
        printf("error ! unsuported input!");
    }
    //free(encInfo.magic_string);
    return 0;
}
}

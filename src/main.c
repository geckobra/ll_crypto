#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hex_to_64.h"

void main(int argc, char* argv[]){

    char* hex_str;
    if(argc == 2){
        hex_str = (char*)malloc(strlen(argv[1]));
        
        memcpy(hex_str, argv[1], strlen(argv[1]));
    }
    size_t raw_len = strlen(hex_str);
    char* base64_str = (char*)malloc(((raw_len + 2) / 3) * 4);

    hex_to_base(hex_str, base64_str, strlen(hex_str));
}
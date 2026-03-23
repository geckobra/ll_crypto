#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hex_to_64.h"
#include "fixed_xor.h"
#include "single_byte_XOR.h"
#include "repeating_key_XOR.h"
#include "base_64_decode.h"

#define NUMBER_LINES 327

void test_cipher_XOR(){
    init_frequencies();

    double overall_max_score = -90000;
    int cipher_character = 0;
    cipher_params cipher_parameters[NUMBER_LINES];
    uint8_t raw_lines[NUMBER_LINES][60];
    int real_line = 0;

    memset(cipher_parameters, 0, sizeof(cipher_params)*60);
    
    char line[128];
    FILE* f = fopen("4.txt", "r");

    for(int i = 0; i<NUMBER_LINES; i++){
        if(fgets(line, 128, f)){
            cipher_xor(line, raw_lines[i], &cipher_parameters[i]);
            if(cipher_parameters[i].max_score > overall_max_score){
                overall_max_score = cipher_parameters[i].max_score;
                cipher_character = cipher_parameters[i].cipher_key;
                real_line = i;
            }
        }
    }

    printf("%s\n", raw_lines[real_line]);
}

void main(int argc, char* argv[]){

    const char* encoded_string = "YW55IGNhcm5hbCBwbGVhc3VyZQ==";

    decode_base64(encoded_string, strlen(encoded_string));
}
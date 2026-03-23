#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static const int8_t b64_table[256] = {
    ['A']=0, ['B']=1, ['C']=2, ['D']=3, ['E']=4, ['F']=5, ['G']=6, ['H']=7,
    ['I']=8, ['J']=9, ['K']=10, ['L']=11, ['M']=12, ['N']=13, ['O']=14, ['P']=15,
    ['Q']=16, ['R']=17, ['S']=18, ['T']=19, ['U']=20, ['V']=21, ['W']=22, ['X']=23,
    ['Y']=24, ['Z']=25, ['a']=26, ['b']=27, ['c']=28, ['d']=29, ['e']=30, ['f']=31,
    ['g']=32, ['h']=33, ['i']=34, ['j']=35, ['k']=36, ['l']=37, ['m']=38, ['n']=39,
    ['o']=40, ['p']=41, ['q']=42, ['r']=43, ['s']=44, ['t']=45, ['u']=46, ['v']=47,
    ['w']=48, ['x']=49, ['y']=50, ['z']=51, ['0']=52, ['1']=53, ['2']=54, ['3']=55,
    ['4']=56, ['5']=57, ['6']=58, ['7']=59, ['8']=60, ['9']=61, ['+']=62, ['/']=63,
    ['=']=-1
};

void decode_base64(const char* base64_data, size_t data_len){
    //this function data that has been encoded in base64 and decodes it to a hex string

    size_t decoded_len = (data_len*3)/4;
    char result_str[decoded_len+1];
    memset(result_str, 0, sizeof(char)*decoded_len);
    int output_idx = 0;

    //iterate through the array taking characters 4 by 4
    for(int i = 0; i<data_len; i+=4){
        
        int v1 = b64_table[(uint8_t)base64_data[i]];
        int v2 = b64_table[(uint8_t)base64_data[i+1]];
        int v3 = b64_table[(uint8_t)base64_data[i+2]];
        int v4 = b64_table[(uint8_t)base64_data[i+3]];

        uint32_t combined_val = (v1<<18) | (v2<<12);

        //extract the values from the combined characters
        result_str[output_idx++] = (combined_val >> 16) & 0xFF;
        
        if (base64_data[i+2] != '='){
            combined_val |= (v3 << 6);
            result_str[output_idx++] = (combined_val >> 8) & 0xFF;
            if(base64_data[i+3] != '='){
                combined_val |= v4;
                result_str[output_idx++] = combined_val & 0xFF;
            }
        }
    }

    result_str[output_idx] = '\0';
    printf("Decoded str: %s\n", result_str);
}
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hex_to_64.h"

void encrypt_RKX(uint8_t* raw_str, const char* key, char* dst, size_t raw_len){
    //take a raw bytes string and encrypt it using repeating key XOR
    
    uint8_t result[raw_len];
    size_t key_len = strlen(key);
    size_t key_idx = 0;

    for(int i = 0; i<raw_len; i++){
        result[i] = raw_str[i] ^ key[key_idx];

        //index the key as a ring buffer so it repeats itself
        key_idx = (key_idx + 1)%key_len;
    }

    raw_to_hex(result, dst, raw_len);
}
#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "hex_to_64.h"
//fixed XOR of raw bytes

void fixed_xor(char* hex_str1, char* hex_str2, size_t ss1) {
    size_t byte_len = ss1 / 2;
    uint8_t raw1[byte_len];
    uint8_t raw2[byte_len];

    hex_to_raw(hex_str1, raw1, ss1);
    hex_to_raw(hex_str2, raw2, ss1);

    for(size_t i = 0; i < byte_len; i++) {
        raw1[i] ^= raw2[i];
    }

    char result_hex[ss1 + 1]; 
    raw_to_hex(raw1, result_hex, byte_len);

    printf("%s\n", result_hex);
}
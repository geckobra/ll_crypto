// the code in this file encodes hex into base64
#pragma once
#include <unistd.h>
#include <stdint.h>
#include <string.h>

const char* base64_lookup = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int count_bits(int value){
    int ones = 0;

    while(value != 0){
        value &= (value-1);
        ones++;
    }

    return ones;
}

int hamming_distance(const char* str1, const char* str2){
    size_t str_length = strlen(str1);

    int hamming_distance = 0;
    for(int i = 0; i<str_length; i++){
        int xor_value = str1[i] ^ str2[i];

        hamming_distance += count_bits(xor_value);
    }

    return hamming_distance;
}

void hex_dump(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int hex_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

void hex_to_raw(char* hex_str, uint8_t* raw_data, size_t hex_size) {
    for (size_t i = 0; i < hex_size / 2; i++) {
        int high = hex_val(hex_str[i * 2]);
        int low  = hex_val(hex_str[i * 2 + 1]);
        
        // If either nibble is invalid, we stop or handle error
        if (high == -1 || low == -1) break; 
        
        raw_data[i] = (uint8_t)((high << 4) | low);
    }
}

void raw_to_hex(uint8_t* raw_bytes, char* hex_str, size_t raw_size){
    const char* hex_lookup = "0123456789abcdef";
    size_t hex_size = raw_size * 2;

    size_t i = 0;
    for(; i<raw_size; i++){
        hex_str[i*2] = hex_lookup[(raw_bytes[i] >> 4) & 0x0F];
        hex_str[i*2+1] = hex_lookup[raw_bytes[i] & 0x0F];
    }

    hex_str[raw_size*2] = '\0';
}

void hex_to_base(char* hex_str, char* base64_buf, size_t hex_size){
    //first, the hex string is converted into raw bytes upon which to operate
    uint8_t raw_data[hex_size/2];
    hex_to_raw(hex_str, raw_data, hex_size);
    
    //iterate the raw data in chunks of 3 bytes (24 bits) and encode based on that
    int output_index = 0;
    for(int i = 0; i<hex_size/2; i+=3){
        
        uint32_t value = 0;
        value = ((uint32_t)raw_data[i] << 16);

        uint32_t remaining_bytes = (hex_size/2) - i;

        //these two values from the lookup are always encoded even if padding is needed
        base64_buf[output_index] = base64_lookup[value>>18 & 0x3F];

        if(remaining_bytes > 1) value |= (uint32_t)raw_data[i + 1] << 8;
        if(remaining_bytes > 2) value |= (uint32_t)raw_data[i + 2];

        base64_buf[output_index++] = base64_lookup[value>>18 & 0x3F];
        base64_buf[output_index++] = base64_lookup[value>>12 & 0x3F];

        if(remaining_bytes>1){
            base64_buf[output_index++] = base64_lookup[value>>6 & 0x3F];
        }
        else{
            base64_buf[output_index++] = '=';
        }

        if(remaining_bytes>2){
            base64_buf[output_index++] = base64_lookup[value & 0x3F];
        }
        else{
            base64_buf[output_index++] = '=';
        }
    }

    base64_buf[output_index] = '\0';
}

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hex_to_64.h"

double english_frequencies[256];

typedef struct cipher_vals{
    double max_score;
    int cipher_key;
} cipher_params;

void init_frequencies() {

    for(int i = 0; i < 256; i++) {
        english_frequencies[i] = -10.0;
    }

    for(int i = 32; i < 127; i++) {
        english_frequencies[i] = 0.0; 
    }

    for(int i = '0'; i <= '9'; i++) {
        english_frequencies[i] = -5.0; 
    }

    english_frequencies[' '] = 10.00;
    english_frequencies['e'] = 12.70; english_frequencies['E'] = 12.70;
    english_frequencies['t'] = 9.06;  english_frequencies['T'] = 9.06;
    english_frequencies['a'] = 8.17;  english_frequencies['A'] = 8.17;
    english_frequencies['o'] = 7.51;  english_frequencies['O'] = 7.51;
    english_frequencies['i'] = 6.97;  english_frequencies['I'] = 6.97;
    english_frequencies['n'] = 6.75;  english_frequencies['N'] = 6.75;
    english_frequencies['s'] = 6.33;  english_frequencies['S'] = 6.33;
    english_frequencies['h'] = 6.09;  english_frequencies['H'] = 6.09;
    english_frequencies['r'] = 5.99;  english_frequencies['R'] = 5.99;
    english_frequencies['m'] = 2.4; english_frequencies['M'] = 2.4;
    english_frequencies['u'] = 2.8; english_frequencies['U'] = 2.8;
    english_frequencies['g'] = 2.0; english_frequencies['G'] = 2.0;
    english_frequencies['p'] = 1.9; english_frequencies['P'] = 1.9;
    english_frequencies['l'] = 4.0; english_frequencies['L'] = 4.0;
}

double cipher_xor(char* hex_str, uint8_t* raw_dst, cipher_params* result){

    size_t bytes_length = strlen(hex_str)/2;
    uint8_t raw_bytes[bytes_length];
    uint8_t result_raw[bytes_length+1];
    char result_string[strlen(hex_str)];

    hex_to_raw(hex_str, raw_bytes, strlen(hex_str));

    double scores[256]; //store a score for each character
    memset(scores, 0, sizeof(double)*256);
    int best_key = 0;
    double max_score = -1e15;
 
    for(int i = 0; i< 256; i++){
        for(int j = 0; j<bytes_length; j++){
            int value = raw_bytes[j] ^ i;

            if (value < 32 || value > 126) {
                if (value != '\n' && value != '\r' && value != '\t') {
                    scores[i] -= 50.0;
                }
            }

            scores[i] += english_frequencies[value];
        }
    }

    for(int i = 0; i< 256; i++){
        if(scores[i] > max_score){
            max_score = scores[i];
            best_key = i;
        }
    }

    for(int i = 0; i<bytes_length; i++){
        result_raw[i] = raw_bytes[i] ^ best_key;
    }

    result->cipher_key = best_key;
    result->max_score = max_score;

    memcpy(raw_dst, result_raw, sizeof(uint8_t)*bytes_length + 1);

    raw_dst[bytes_length] = '\0';
}
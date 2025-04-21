#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

// Constants from XXH3 algorithm (adjusted for 32-bit)
#define PRIME32_1 0x9E3779B1U  // 2654435761
#define PRIME32_2 0x85EBCA77U  // 2246822519
#define PRIME32_3 0xC2B2AE3DU  // 3266489917
#define PRIME32_4 0x27D4EB2FU  // 668265263

const __m256i prime1 = _mm256_set1_epi32(PRIME32_1);
const __m256i prime2 = _mm256_set1_epi32(PRIME32_2);
const __m256i prime3 = _mm256_set1_epi32(PRIME32_3);
const __m256i prime4 = _mm256_set1_epi32(PRIME32_4);

#define SECRET_SIZE 16

static const uint32_t SECRET[SECRET_SIZE/4] = {
    0x9E3779B1, 0x85EBCA77, 0xC2B2AE3D, 0x27D4EB2F
};

__m256i XXH32_avalanche(__m256i h32){
    // h32 ^= h32 >> 15;
    h32 = _mm256_xor_si256(h32, _mm256_srai_epi32(h32, 15));
    // h32 *= PRIME32_2;
    h32 = _mm256_mul_epi32(h32, prime2);
    // h32 ^= h32 >> 13;
    h32 = _mm256_xor_si256(h32, _mm256_srai_epi32(h32, 13));
    // h32 *= PRIME32_3;
    h32 = _mm256_mul_epi32(h32, prime3);
    // h32 ^= h32 >> 16;
    h32 = _mm256_xor_si256(h32, _mm256_srai_epi32(h32, 13));
    return h32;
}

uint32_t XXH3_hash32(uint32_t input, uint32_t seed){

    uint32_t acc  = seed + PRIME32_1;
    uint32_t lane = input;

    uint32_t keyed = lane ^ (SECRET[0] + seed);
    acc += keyed * PRIME32_2;
    acc = (acc << 13) | (acc >> 19);

    acc ^= (acc >> 15);
    acc *= PRIME32_2;
    acc ^= (acc >> 13);
    acc *= PRIME32_3;
    acc ^= (acc >> 16);

    return acc;
}

uint32_t XXH3_avalanche32(uint32_t input) {
    uint32_t hash = input + PRIME32_4;
    hash = nonaligned_hash((char*) &hash);
    return hash;
}

uint32_t nonaligned_hash(char* str){
   char word[32] = {};
   strcpy(word, str);
   return xxh3_hash(word);
}

uint32_t xxh3_hash(const char *str){ //use 32 chars

    uint32_t hash = PRIME32_4;
    alignas(64) uint32_t list[8] = {};

    __m256i word  = _mm256_load_epi32(str);
    word = XXH32_avalanche(word);
    _mm256_store_epi32(list, word);
    hash = (PRIME32_1*list[0] + PRIME32_4*list[1] + PRIME32_2*list[2]) % TABLE_SIZE + list[3] + list[4]+ list[5]+ list[6] + list[7];
    return hash % TABLE_SIZE;
}

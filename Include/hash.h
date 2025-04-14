#include <stdint.h>
#include <immintrin.h>
#include <ctype.h>

#define TABLE_SIZE 65536/2

__m256i XXH32_avalanche(__m256i h32);
uint32_t XXH3_avalanche32(uint32_t h32);
uint32_t XXH3_hash32(uint32_t input, uint32_t seed);
uint32_t nonaligned_hash(char* str);
uint32_t xxh3_hash(const char *str);

typedef struct HashNode {
    char *word;
    int count;
    struct HashNode *next;
} HashNode;


#include <stdint.h>
#include <immintrin.h>
#include <ctype.h>

#define TABLE_SIZE 1024

uint32_t slowhash(char* word);
__m256i XXH32_avalanche(__m256i h32);
uint32_t XXH3_avalanche32(uint32_t h32);
uint32_t XXH3_hash32(uint32_t input, uint32_t seed);
uint32_t nonaligned_hash(char* str);
uint32_t xxh3_hash(const char *str);

#ifdef NOHASH
    #define HASH1(a) slowhash(a)
    #define HASH2(a) slowhash(a)
#else
    #define HASH1(a) xxh3_hash(a)
    #define HASH2(a) nonaligned_hash(a)
#endif

typedef struct HashNode {
    char *word;
    int count;
    struct HashNode *next;
} HashNode;


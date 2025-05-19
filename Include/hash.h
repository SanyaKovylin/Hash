#include <stdint.h>
#include <immintrin.h>
#include <ctype.h>

#define TABLE_SIZE 1009

uint32_t crc32(void *data, size_t len);
uint32_t hash_crc32(char *s);
uint32_t asm_hash_crc32(const void *data);
uint32_t nonaligned(const char* s);
uint32_t asm_hash_crc32_simd(__m256i data);

#ifdef NOHASH
    #define HASH1(a) hash_crc32(a)
    #define HASH2(a) nonaligned1(a)
#else
    #define HASH1(a) asm_hash_crc32(a)
    #define HASH2(a) nonaligned(a)
#endif

typedef struct HashNode {
    char *word;
    int count;
    int wordlen;
    struct HashNode *next;
} HashNode;

typedef struct HashNodeSIMD {
    __m256i word;
    struct HashNodeSIMD *next;
} HashNodeSIMD;


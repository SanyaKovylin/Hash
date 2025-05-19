#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <immintrin.h>

#include "hash.h"
#include "process.h"

#define iterations 100

HashNode *Table[TABLE_SIZE] = {NULL};
HashNodeSIMD *TableSIMD[TABLE_SIZE] = {NULL};
HashNode *Nodes = NULL;

int find(char *word);
int new_find(__m256i word);
alignas(32) char word[32] = {};

uint32_t nonaligned2(const char* s){

    strcpy(word, s);
    for (int i = 0; i < 32; i++){
        printf("(%c)", word[i]);
    }
    return asm_hash_crc32_simd(_mm256_load_epi32(word));
}

#define buf (*storage)

void load_table(const char* filename, char** storage, size_t* len){

    *len = BaseRead(filename, storage) >> 5 << 5;
    size_t ptr = 0;

    assert(buf != NULL);
    Nodes = (HashNode*) calloc ((*len >> 5) + 1, sizeof(HashNode));

    size_t last = 0;
    while (ptr < *len - 1){
        size_t word_len = 0;

        while (buf[ptr + word_len] > 0){
            word_len++;
        }

        HashNode *new_node = Nodes + (ptr >> 5);
        assert(new_node != NULL);
        new_node->word = buf + ptr;
        new_node->count = *(int*)(buf + ptr + word_len + 1);
        new_node->wordlen = word_len;

        memset(buf + ptr + word_len, 0, (32 - word_len)*sizeof(char));
        uint32_t index = HASH1(buf + ptr);
        if (index != last)
            Table[last] = new_node - 1;
        else
            new_node->next = new_node - 1;

        last = index;
        ptr += 32;
    }
    Table[last] = Nodes + ((ptr >> 5) - 1);
    Nodes->next = NULL;
}

void simd_load_table(const char* filename, char** storage, size_t* len){

    *len = BaseRead(filename, storage) >> 5 << 5;
    size_t ptr = 0;

    assert(buf != NULL);
    HashNodeSIMD *Nodes = (HashNodeSIMD*) calloc ((*len >> 5) + 1, sizeof(HashNodeSIMD));

    size_t last = 0;
    while (ptr < *len - 1){

        HashNodeSIMD *new_node = Nodes + (ptr >> 5);
        assert(new_node != NULL);

        new_node->word = _mm256_load_epi32(buf + ptr);
        for (int i = 0; i < 32; i++){
            printf("(%c)", (buf + ptr)[i]);
        }
        uint32_t index = asm_hash_crc32_simd(_mm256_load_epi32(buf + ptr));
        printf("%d \n", HASH2(buf + ptr));
        printf("%d \n", index);
        if (index != last)
            TableSIMD[last] = new_node - 1;
        else
            new_node->next = new_node - 1;

        last = index;
        ptr += 32;
    }

    TableSIMD[last] = Nodes + ((ptr >> 5) - 1);
    Nodes->next = NULL;
}

void prev_load_table(const char* filename, char** storage, size_t* len){

    *len = BaseRead(filename, storage) >> 5 << 5;
    size_t ptr = 0;

    assert(buf != NULL);
    Nodes = (HashNode*) calloc ((*len >> 5) + 1, sizeof(HashNode));
    // puts("started");
    while (ptr < *len - 1){
        size_t word_len = 0;

        while (buf[ptr + word_len] > 0){
            word_len++;
        }

        HashNode *new_node = (HashNode*) calloc ((*len >> 5) + 1, sizeof(HashNode));
        assert(new_node != NULL);
        new_node->word = buf + ptr;
        new_node->count = *(int*)(buf + ptr + word_len + 1);
        memset(buf + ptr + word_len, 0, (32 - word_len)*sizeof(char));
        uint32_t index = HASH1(buf + ptr);

        new_node->next = Table[index];
        Table[index] = new_node;

        ptr += 32;
    }
}

#undef buf

size_t BaseRead (const char *src, char** Buffer) {

    assert(Buffer != NULL);
    int fo = open (src, O_RDONLY);
    struct stat st = {};

    stat (src, &st);
    size_t readlen = (size_t) st.st_size;

    size_t to_allocate = (((readlen >> 6) + 1) << 6);
    *Buffer = (char*) aligned_alloc (64, readlen * sizeof (char));
    assert(*Buffer != NULL);

    size_t lenbuf = read (fo, *Buffer, readlen);
    return lenbuf;
}

int find(char *word){

    uint32_t index = HASH1(word);
    HashNode *node = Table[index];
    while (node != NULL) {
        if (STRCMP(word, node->word) == 0) {
            return node->count;
        }
        node = node->next;
    }

    return 0;
}

__mmask8 mask = _mm256_cmpeq_epi32_mask (_mm256_set1_epi32(0), _mm256_set1_epi32(0));

int new_find(__m256i word){

    uint32_t index = asm_hash_crc32_simd(word);
    HashNodeSIMD *node = TableSIMD[index];
    printf("%d\n", index);
    while (node != NULL) {
        // puts(node->word);
        // puts(word);
        printf("%d %d\n", mask, _mm256_cmpeq_epi32_mask (word, node->word));
        if (_mm256_cmpeq_epi32_mask (word, node->word) == mask) return 1;
        node = node->next;
    };

    return 0;
}

void usage_case(char* buffer, size_t len){
    size_t cnt = 0;
    for (int i = 0;i < iterations; i++){
        int x = FIND(buffer + (i % (len >> 5)) * 32);
        if (!x){
            puts("Abort");
            printf("%s\n", buffer + (i % (len >> 5)) * 32);

        }
        cnt += x;

    }
    printf("%d", cnt);
}

// void usage_case_simd(char* buffer, size_t len){
//     size_t cnt = 0;
//     for (int i = 0;i < iterations; i++){
//         __m256i to_find = _mm256_load_epi32(buffer + (i % (len >> 5)) * 32);
//         int x = FIND(to_find);
//         if (!x){
//             puts("Abort");
//             printf("%s\n", buffer + (i % (len >> 5)) * 32);
//
//         }
//         cnt += x;
//
//     }
//     printf("%d", cnt);
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "process.h"

HashNode *hashTable[TABLE_SIZE] = {NULL};

uint32_t nonaligned1(const char* s){
    alignas(64) char word[32] = {};
    strcpy(word, s);
    return hash_crc32(word);
}

uint32_t nonaligned(const char* s){
    alignas(64) char word[64] = {};
    strcpy(word, s);
    return asm_hash_crc32(word);
}

uint32_t nonaligned2(const char* s){
    alignas(32) char word[32] = {};
    strcpy(word, s);
    return asm_hash_crc32_simd(_mm256_load_epi32(word));
}

void process_word(char *word){

    uint32_t hash = HASH2(word);
    uint32_t index = hash;

    HashNode *node = hashTable[index];
    while (node) {
        if (strcmp(node->word, word) == 0) {
            node->count++;
            return;
        }
        node = node->next;
    }

    HashNode *new_node = (HashNode*) calloc(1, sizeof(HashNode));
    new_node->word = strdup(word);
    new_node->count = 1;
    new_node->next = hashTable[index];
    hashTable[index] = new_node;
}

void build_hash_table(const char *filename){

    size_t mx = 0;
    size_t long_cnt = 0;
    FILE *file = fopen(filename, "r");
    char buffer[1024];
    int words = 0;
    while (fscanf(file, "%1023s ", buffer) == 1){
        proc(buffer);
        process_word(buffer);
        if (strlen(buffer) > mx){
            mx = strlen(buffer);
        }
        words++;
        if (strlen(buffer) > 12) long_cnt++;
    }
    printf("Number of words: %ld, MaxLength: %ld, Number of long words: %ld\n",words, mx, long_cnt);
    fclose(file);
}

void write_output(const char *filename){

    FILE *file = fopen(filename, "w");
    char cell[32] = {};
    int cnt = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = hashTable[i];
        while (node) {
            memset(cell, 0, 32 * sizeof(char));
            sprintf(cell, "%s%c", node->word, 0);
            *(uint32_t*)(cell + strlen(node->word) + 1) = node->count;
            for (int k = 0; k < 32; k++){
                fputc(cell[k], file);
            }
            cnt++;
            node = node->next;
        }
    }

    printf("LoadFactor = %g, cnt distinct = %d, size = %d\n",
            ((float) (cnt - TABLE_SIZE)) / (float) TABLE_SIZE, cnt, TABLE_SIZE);

    fclose(file);
}

void free_table(){
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = hashTable[i];
        while (node) {
            HashNode *temp = node;
            node = node->next;
            free(temp->word);
            free(temp);
        }
    }
}

void proc(char *buf){
    int i = 0;
    while (isalpha(buf[i])) i++;
    buf[i] = 0;
}

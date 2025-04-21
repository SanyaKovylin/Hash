#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "process.h"

// Constants from XXH3 algorithm
#define PRIME32_1 0x9E3779B1U
#define PRIME32_2 0x85EBCA77U
#define PRIME32_3 0xC2B2AE3DU
#define PRIME32_5 0x165667B1U

HashNode *hashTable[TABLE_SIZE] = {NULL};

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
    while (fscanf(file, "%1023s ", buffer) == 1){
        proc(buffer);
        process_word(buffer);
        if (strlen(buffer) > mx){
            mx = strlen(buffer);
        }
        if (strlen(buffer) > 12) long_cnt++;
    }
    printf("MaxLength: %ld, %ld\n", mx, long_cnt);
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
            sprintf(cell, "%s%c%d", node->word, 0, node->count);
            for (int k = 0; k < 32; k++){
                fputc(cell[k], file);
            }
            cnt++;
            node = node->next;
        }
    }

    printf("LoadFactor = %g, cnt = %d, size = %d\n",
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

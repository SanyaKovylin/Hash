#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hash.h"
#include "process.h"

#define iterations 1000

HashNode *Table[TABLE_SIZE] = {NULL};

int find(char *word);

#define buf (*storage)

void load_table(const char* filename, char** storage, size_t* len){

    *len = BaseRead(filename, storage) >> 5 << 5;
    size_t ptr = 0;

    assert(buf != NULL);
    HashNode *Nodes = (HashNode*) calloc ((*len >> 5) + 1, sizeof(HashNode));

    size_t last = 0;
    while (ptr < *len - 1){
        size_t word_len = 0;

        while (buf[ptr + word_len] > 0){
            word_len++;
        }

        HashNode *new_node = Nodes + (ptr >> 5);
        assert(new_node != NULL);
        new_node->word = buf + ptr;
        new_node->count = atoi(buf + ptr + word_len + 1);

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

void prev_load_table(const char* filename, char** storage, size_t* len){

    *len = BaseRead(filename, storage) >> 5 << 5;
    size_t ptr = 0;

    assert(buf != NULL);
    HashNode *Nodes = (HashNode*) calloc ((*len >> 5) + 1, sizeof(HashNode));
    puts("started");
    while (ptr < *len - 1){
        size_t word_len = 0;

        while (buf[ptr + word_len] > 0){
            word_len++;
            printf("%d", word_len);
        }
        printf("%d", ptr);
        HashNode *new_node = Nodes + (ptr >> 5);
        assert(new_node != NULL);
        new_node->word = buf + ptr;
        new_node->count = atoi(buf + ptr + word_len + 1);
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

    *Buffer = (char*) aligned_alloc (64, readlen + 1 * sizeof (char));
    assert(*Buffer != NULL);

    size_t lenbuf = read (fo, *Buffer, readlen);
    return lenbuf;
}

int find(char *word){

    uint32_t index = HASH2(word);
    HashNode *node = Table[index];

    while (node != NULL) {
        if (STRCMP(word, node->word) == 0) {
            return node->count;
        }
        node = node->next;
    }

    return 0;
}

void usage_case(char* buffer, size_t len){
    size_t cnt = 0;
    for (int i = 0;i < iterations; i++){
        int x = find(buffer + (i % (len >> 5))*32);
        if (!x){
            puts("Abort");
            printf("%s\n", buffer + (i % (len >> 5))*32);

        }
        cnt += x;

    }
    printf("%d", cnt);
}

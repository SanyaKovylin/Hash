#include <string.h>
// #include "hash.h"
void process_word(const char *word);
void proc(char *buf);

void write_output(const char *filename);
void build_hash_table(const char *filename);
void free_table();

void load_table(const char* filename, char** storage, size_t* len);
void usage_case(char* buffer, size_t len);
void usage_case_simd(char* buffer, size_t len);

size_t BaseRead (const char *src, char** Buffer);
void prev_load_table(const char* filename, char** storage, size_t* len);
void simd_load_table(const char* filename, char** storage, size_t* len);

extern "C" {
    int mystrcmp(const char* word, const char* tablestring);
}



#ifdef NOSTRCMP
    #define STRCMP(a, b) strcmp(a, b)
#else
    #define STRCMP(a, b) mystrcmp(a, b)
#endif

#ifdef NOALIGN
    #define LOAD(a, b, c) prev_load_table(a, b, c)
#else
    #define LOAD(a, b, c) load_table(a, b, c)

#endif

#ifdef NOSTRCPY
    #define STRCPY(a, b) strcpy(a, b)
#else
    #define STRCPY(a, b) my_strcpy(a, b)
#endif

#undef NOFIND
#ifdef NOFIND
    #define FIND(a) find(a)
    #define US_CASE(a, b) usage_case(a, b)
#else
    #undef LOAD
    #define LOAD(a,b,c) simd_load_table(a,b,c)
    #undef HASH2
    #define HASH2(a) nonaligned2(a)
    #define US_CASE(a, b) usage_case(a, b)
    #define FIND(a) new_find(_mm256_load_epi32(a))
#endif



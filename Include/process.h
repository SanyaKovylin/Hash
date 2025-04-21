#include <string.h>
void process_word(const char *word);
void proc(char *buf);

void write_output(const char *filename);
void build_hash_table(const char *filename);
void free_table();

void load_table(const char* filename, char** storage, size_t* len);
void usage_case(char* buffer, size_t len);

size_t BaseRead (const char *src, char** Buffer);
void load_tests(char* Buffer, char** tests, size_t* lenbuf);
void prev_load_table(const char* filename, char** storage, size_t* len);

extern "C" {
    int mystrcmp(const char* word, const char* tablestring);
}

#ifdef NOSTRCMP
    #define STRCMP(a, b) strcmp(a, b)
#else
    #define STRCMP(a, b) mystrcmp(a, b)
#endif

#ifdef NOLOAD
    #define LOAD(a, b, c) prev_load_table(a, b, c)
#else
    #define LOAD(a, b, c) load_table(a, b, c)
#endif

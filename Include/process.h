void process_word(const char *word);
void proc(char *buf);

void write_output(const char *filename);
void build_hash_table(const char *filename);
void free_table();

void load_table(const char* filename, char** storage);
// void load_tests(const char* filename);
void usage_case(const char* src);
void usage_case2();

size_t BaseRead (const char *src, char** Buffer);
void load_tests(char* Buffer, char** tests, size_t lenbuf);

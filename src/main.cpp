#include <stdlib.h>
#include <stdio.h>

// #include "hash.h"
#include "process.h"


int main() {

    char *WordStore = NULL;
    size_t len = 0;
    LOAD("table/output.txt", &WordStore, &len);
    puts("Load finished");
    US_CASE(WordStore, len);

    return 0;
}

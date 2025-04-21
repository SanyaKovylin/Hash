#include <stdlib.h>
#include <stdio.h>

#include "process.h"
#include "hash.h"

int main() {

    // build_hash_table("txtsrc/input.txt");
    // write_output("table/output.txt");
    // free_table();

    char *WordStore = NULL;
    size_t len = 0;
    load_table("table/output.txt", &WordStore, &len);
    // puts(WordStore);
    usage_case3(WordStore, len);

    return 0;
}

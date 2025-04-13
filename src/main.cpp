#include <stdlib.h>
#include <stdio.h>

#include "process.h"
#include "hash.h"

int main() {

    build_hash_table("txtsrc/input.txt");
    write_output("table/output.txt");
    free_table();


    char *WordStore = NULL;
    // load_table("table/output.txt");
    // puts("1");
    // usage_case2("txtsrc/test2.txt");

    return 0;
}
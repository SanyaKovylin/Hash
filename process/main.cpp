#include <stdlib.h>
#include <stdio.h>

#include "hash.h"
#include "process.h"


int main() {

    build_hash_table("txtsrc/input.txt");
    write_output("table/output.txt");
    free_table();

    return 0;
}

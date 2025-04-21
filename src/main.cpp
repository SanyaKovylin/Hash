#include <stdlib.h>
#include <stdio.h>

#include "process.h"
#include "hash.h"

int main() {

//     char w[32] = {
//         'D','e','e','p' ,'v','a','l','l'   ,'e','y','f','o',  'r','e','s','t',  'e','d',0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0
//     };
//
//     char e[] = "Deepvalleyforested";
//     printf("%d", STRCMP(e,w));

    char *WordStore = NULL;
    size_t len = 0;
    LOAD("table/output.txt", &WordStore, &len);
    puts("Load finished");
    usage_case(WordStore, len);

    return 0;
}

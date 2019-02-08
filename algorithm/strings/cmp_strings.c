#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparer_chaines(char ** strs, int n, char * str) {
    int i;
    for(i = 0; i < n; i++) {
        if(strcmp(strs[i], str) == 0)
            return 1;
    }
    return 0;
}
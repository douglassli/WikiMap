#include <stdlib.h>
#include <stdio.h>

#include "map_vec.h"
#include "node.h"


map_vec* parse_map_file(char* file) {
    FILE* fp = fopen(file, "r");
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while (1) {
        linelen = getline(&line, &linecap, fp);
        fwrite(line, linelen, 1, stdout);
        if (line[0] == '*') {
            break;
        }
    }
    return 0;
}


int main() {
    char* file = "map.txt";
    parse_map_file(file);
    return 0;
}


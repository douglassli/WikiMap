#include <stdlib.h>
#include <stdio.h>

#include "map_vec.h"
#include "node.h"


map_vec* parse_map_file(char* file) {
    int fp = open(file, O_RDONLY);
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while (1) {
        linelen = getline(&line, &linecap, fp);
        if (line[0] == '*') {
            break;
        }
        fwrite(line, linelen, 1, stdout);
    }
    return make_map();
}


int main() {
    char* file = "map.txt";
    parse_map_file(file);
    return 0;
}


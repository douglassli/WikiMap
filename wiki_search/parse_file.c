#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map_vec.h"
#include "node.h"


map_vec* parse_map_file(char* file) {
    FILE* fp = fopen(file, "r");
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while (1) {
        linelen = getline(&line, &linecap, fp);
        if (line[0] == '*') {
            break;
        }

        char* nums;
        nums = strtok(line, " ");
        
        long counter = 0;
        node* node;

        while(nums != NULL) {
            long val = atol(nums);
            if (counter == 0) {
                node = make_node(val);
            }
            else {
                push_node(node, val);
            }
            printf("%s\n", nums);
            nums = strtok(NULL, " ");
        }
    }
    return 0;
}


int main() {
    char* file = "map.txt";
    parse_map_file(file);
    return 0;
}


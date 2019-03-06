#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map_vec.h"
#include "node.h"


node* parse_line(char* line) {
    char* nums;
    nums = strtok(line, " ");
    
    long node_val = atol(nums);
    printf("Node: %ld\n", node_val);
    node* node = make_node(node_val);
    nums = strtok(NULL, " ");

    while(nums != NULL) {
        if (nums[0] == '\n') {
            break;
        }
        long val = atol(nums);
        push_node(node, val);
        nums = strtok(NULL, " ");
    }

    return node;
}


map_vec* parse_map_file(char* file) {
    FILE* fp = fopen(file, "r");
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    map_vec* map = make_map();

    while (1) {
        linelen = getline(&line, &linecap, fp);
        if (line[0] == '*' || linelen <= 0) {
            break;
        }

        node* node = parse_line(line);

        push_map(map, node);
    }
    return map;
}


int main() {
    char* file = "map.txt";
    map_vec* map = parse_map_file(file);
    print_map(map);
    return 0;
}


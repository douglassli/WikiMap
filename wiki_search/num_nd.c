#include <stdlib.h>
#include <stdio.h>

#include "map_vec.h"
#include "node.h"
#include "parse_file.h"

int main() {
    map_vec* map = parse_map_file("med2_map.txt");
    long num_nodes = 0;
    long num_edges = 0;
    for (long i = 0; i < map->size; i++) {
        num_nodes++;
        node* n = &map->nodes[i];
        num_edges += n->size;
    }
    printf("Map has %ld nodes and %ld edges\n", num_nodes, num_edges);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfs.h"
#include "dijkstra.h"
#include "parse_file.h"

int main(int argc, char* argv[]) {
    if (strcmp(argv[1], "bfs") == 0 && argc == 5) {
        printf("PARSING...\n");
        map_vec* map = parse_map_file(argv[2]);
        printf("SEARCHING...");
        bfs(map, atol(argv[3]), atol(argv[4]));
        return 0;
    } else if (strcmp(argv[1], "dijkstra") == 0 && argc == 4) {
        printf("Parsing...\n");
        map_vec* map = parse_map_file(atol(argv[2]));
        printf("SEARCHING...\n");
        dijkstra(map, argv[3]);
        return 0;
    }

    printf("INVALID ARGUMENTS\n");
    return 1;
}

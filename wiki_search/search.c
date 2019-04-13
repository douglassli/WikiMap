#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfs.h"
#include "dijkstra.h"
#include "parse_file.h"
#include "par_bfs.h"

int main(int argc, char* argv[]) {
    if (strcmp(argv[1], "bfs") == 0 && argc == 5) {
        printf("Running Breadth First Search\n");
        printf(" - Input File: %s\n", argv[2]);
        printf(" - Source:     %ld\n", atol(argv[3]));
        printf(" - Goal:       %ld\n\n", atol(argv[4]));
        printf("Parsing input file...\n");
        map_vec* map = parse_map_file(argv[2]);
        printf("Finished parsing.\n\n");
        printf("Starting search...\n");
        bfs(map, atol(argv[3]), atol(argv[4]));
        return 0;
    } else if (strcmp(argv[1], "dijkstra") == 0 && argc == 4) {
        printf("Running Dijkstra's Algorithm\n");
        printf(" - Input File: %s\n", argv[2]);
        printf(" - Source:     %ld\n\n", atol(argv[3]));
        printf("Parsing input file...\n");
        map_vec* map = parse_map_file(argv[2]);
        printf("Finished parsing.\n\n");
        printf("Starting search...\n");
        dijkstra(map, atol(argv[3]));
        return 0;
    } else if (strcmp(argv[1], "pbfs") == 0 && argc == 5) {
        printf("Running Parallel Breadth First Search\n");
        printf(" - Input file:  %s\n", argv[2]);
        printf(" - Source:      %ld\n", atol(argv[3]));
        printf(" - Num Threads: %d\n\n", atoi(argv[4]));
        printf("Parsing input file...\n");
        map_vec* map = parse_map_file(argv[2]);
        printf("Finished parsing.\n\n");
        printf("Starting search...\n");
        par_bfs(map, atol(argv[3]), atoi(argv[4]));
        return 0;
    }

    printf("INVALID ARGUMENTS\n");
    return 1;
}

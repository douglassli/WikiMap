#include <stdio.h>
#include <stdlib.h>

#include "bfs.h"
#include "parse_file.h"

int main(int argc char* argv[]) {
    if (strcmp(argv[1], "bfs") && argc == 5) {
        printf("PARSING...\n");
        map_vec* map = parse_map_file(argv[2]);
        printf("SEARCHING...");
        bfs(map, atol(argv[3]), atol(argv[4]));
        return 0;
    }

    printf("INVALID ARGUMENTS\n");
    return 1;
}

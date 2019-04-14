#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bfs.h"
#include "dijkstra.h"
#include "parse_file.h"
#include "par_bfs.h"

double get_seconds(struct timespec start, struct timespec stop) {
    return (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
}

int main(int argc, char* argv[]) {
    struct timespec start, stop;

    if (strcmp(argv[1], "bfs") == 0 && argc == 5) {
        printf("Running Breadth First Search\n");
        printf(" - Input File: %s\n", argv[2]);
        printf(" - Source:     %ld\n", atol(argv[3]));
        printf(" - Goal:       %ld\n\n", atol(argv[4]));
        printf("Parsing input file...\n");

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        map_vec* map = parse_map_file(argv[2]);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        
        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, stop);
        printf("Starting search...\n");
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        bfs(map, atol(argv[3]), atol(argv[4]));
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        printf("\nElapsed search time: %f\n\n", get_seconds(start, stop);
        
        return 0;
    } else if (strcmp(argv[1], "dijkstra") == 0 && argc == 4) {
        printf("Running Dijkstra's Algorithm\n");
        printf(" - Input File: %s\n", argv[2]);
        printf(" - Source:     %ld\n\n", atol(argv[3]));
        printf("Parsing input file...\n");
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        map_vec* map = parse_map_file(argv[2]);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        
        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, stop);
        printf("Starting search...\n");
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        dijkstra(map, atol(argv[3]));
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        printf("\nElapsed search time: %f\n\n", get_seconds(start, stop);
        
        return 0;
    } else if (strcmp(argv[1], "pbfs") == 0 && argc == 5) {
        printf("Running Parallel Breadth First Search\n");
        printf(" - Input file:  %s\n", argv[2]);
        printf(" - Source:      %ld\n", atol(argv[3]));
        printf(" - Num Threads: %d\n\n", atoi(argv[4]));
        printf("Parsing input file...\n");
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        map_vec* map = parse_map_file(argv[2]);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        
        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, stop);
        printf("Starting search...\n");
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        par_bfs(map, atol(argv[3]), atoi(argv[4]));
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        printf("\nElapsed search time: %f\n\n", get_seconds(start, stop);
        
        return 0;
    }

    printf("INVALID ARGUMENTS\n");
    return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "bfs.h"
#include "dijkstra.h"
#include "parse_file.h"
#include "par_bfs.h"
#include "par_bfs2.h"
#include "par_bfs3.h"

double get_seconds(struct timeval start, struct timeval end) { 
    return (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1e6;
}

int main(int argc, char* argv[]) {
    struct timeval start, end;
    int print_output = 1;
    if (strcmp(argv[argc - 1], "-np") == 0) {
        print_output = 0;
        argc--;
    }

    if (strcmp(argv[1], "bfs") == 0 && argc == 5) {
        printf("Running Breadth First Search\n");
        printf(" - Input File: %s\n", argv[2]);
        printf(" - Source:     %ld\n", atol(argv[3]));
        printf(" - Goal:       %ld\n\n", atol(argv[4]));
        printf("Parsing input file...\n");

        gettimeofday(&start, NULL);
        map_vec* map = parse_map_file(argv[2]);
        gettimeofday(&end, NULL);

        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, end));
        printf("Starting search...\n");
 
        gettimeofday(&start, NULL);       
        bfs(map, atol(argv[3]), atol(argv[4]));
        gettimeofday(&end, NULL);
        printf("\nElapsed search time: %f\n\n", get_seconds(start, end));
        
        return 0;
    } else if (strcmp(argv[1], "dijkstra") == 0 && argc == 4) {
        printf("Running Dijkstra's Algorithm\n");
        printf(" - Input File: %s\n", argv[2]);
        printf(" - Source:     %ld\n\n", atol(argv[3]));
        printf("Parsing input file...\n");
        
        gettimeofday(&start, NULL);
        map_vec* map = parse_map_file(argv[2]);
        gettimeofday(&end, NULL);
        
        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, end));
        printf("Starting search...\n");
        
        gettimeofday(&start, NULL);
        dijkstra(map, atol(argv[3]), print_output);
        gettimeofday(&end, NULL);
        printf("\nElapsed search time: %f\n\n", get_seconds(start, end));
        
        return 0;
    } else if (strcmp(argv[1], "pbfs") == 0 && argc == 5) {
        printf("Running Parallel Breadth First Search\n");
        printf(" - Input file:  %s\n", argv[2]);
        printf(" - Source:      %ld\n", atol(argv[3]));
        printf(" - Num Threads: %d\n\n", atoi(argv[4]));
        printf("Parsing input file...\n");
        
        gettimeofday(&start, NULL);
        map_vec* map = parse_map_file(argv[2]);
        gettimeofday(&end, NULL);
        
        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, end));
        printf("Starting search...\n");
        
        gettimeofday(&start, NULL);
        par_bfs(map, atol(argv[3]), atoi(argv[4]), print_output);
        gettimeofday(&end, NULL);
        printf("\nElapsed search time: %f\n\n", get_seconds(start, end));
        
        return 0;
    } else if (strcmp(argv[1], "pbfs2") == 0 && argc == 5) {
        printf("Running Parallel Breadth First Search Version 2\n");
        printf(" - Input file:  %s\n", argv[2]);
        printf(" - Source:      %ld\n", atol(argv[3]));
        printf(" - Num Threads: %d\n\n", atoi(argv[4]));
        printf("Parsing input file...\n");

        gettimeofday(&start, NULL);
        map_vec* map = parse_map_file(argv[2]);
        gettimeofday(&end, NULL);

        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, end));
        printf("Starting search...\n");

        gettimeofday(&start, NULL);
        par_bfs2(map, atol(argv[3]), atoi(argv[4]), print_output);
        gettimeofday(&end, NULL);
        printf("\nElapsed search time: %f\n\n", get_seconds(start, end));

        return 0;
    } else if (strcmp(argv[1], "pbfs3") == 0 && argc == 5) {
        printf("Running Parallel Breadth First Search Version 3\n");
        printf(" - Input file:  %s\n", argv[2]);
        printf(" - Source:      %ld\n", atol(argv[3]));
        printf(" - Num Threads: %d\n\n", atoi(argv[4]));
        printf("Parsing input file...\n");

        gettimeofday(&start, NULL);
        map_vec* map = parse_map_file(argv[2]);
        gettimeofday(&end, NULL);

        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, end));
        printf("Starting search...\n");

        gettimeofday(&start, NULL);
        par_bfs3(map, atol(argv[3]), atoi(argv[4]), print_output);
        gettimeofday(&end, NULL);
        printf("\nElapsed search time: %f\n\n", get_seconds(start, end));

        return 0;
    } else if (strcmp(argv[1], "benchmark") == 0 && argc == 3) {
        printf("Running benchmark on Dijkstra and PBFS\n");
        printf(" - Input File: %s\n\n", argv[2]);

        printf("Parsing input file...\n");
        gettimeofday(&start, NULL);
        map_vec* map = parse_map_file(argv[2]);
        gettimeofday(&end, NULL);
        
        printf("Finished parsing.\n");
        printf("Elapsed parse time: %f\n\n", get_seconds(start, end));
        /*         
        printf("Benchmarking Dijkstra...\n");
        double total = 0.0;
        for (int i = 0; i < 100; i++) {
            gettimeofday(&start, NULL);
            dijkstra(map, 505, 0);;
            gettimeofday(&end, NULL);
            double secs = get_seconds(start, end);
            printf("%f\n", secs);
            total += secs;
        }
        printf("Average time: %f secs\n", total / 100.0);
        printf("Dijkstra Done.\n");
        */
        for (int n = 1; n < 5; n++) {
            printf("\nBenchmarking PBFS %d threads...\n", n);
            double total = 0.0;
            for (int i = 0; i < 100; i++) {
                gettimeofday(&start, NULL);
                par_bfs3(map, 505, n, 0);
                gettimeofday(&end, NULL);
                double secs = get_seconds(start, end);
                printf("%f\n", secs);
                total += secs;
            }
            printf("Average time: %f secs\n", total / 100.0);
            printf("PBFS Done.\n");
        }
        return 0;
    }

    printf("INVALID ARGUMENTS\n");
    return 1;
}

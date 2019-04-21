#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#include "par_bfs4.h"
#include "map_vec.h"
#include "frontier.h"
#include "explored_vec.h"
#include "node.h"
#include "fr_pair.h"
#include "linked_frnts.h"
#include "pthread_barrier.h"

map_vec* global_map4;
explored* global_dists4;
linked_frnts* cur_lfrnt;
linked_frnts* next_lfrnt;
pthread_barrier_t barrier4;
pthread_mutex_t mutex4;

typedef struct job4 {
    int thread_num;
    int num_threads;
} job4;

void succs_to_fr4(frontier* fr, fr_pair* start_node) {
    node* succ_nodes = map_get_node(global_map4, start_node->node_val);
    for (long n = 0; n < succ_nodes->size; n++) {
        long succ = succ_nodes->data[n];
        if (global_dists4->data[succ] == -1) {
            fr_pair* new_frp = new_pair(succ, start_node->dist + 1);
            push_frontier(fr, (long)new_frp);
        }
   }
}

frontier* bfs_worker4(int t_num, int num_threads) {
    frontier* out_fr = make_frontier();
    long num_expanded = 0;

    long index = 0;
    long fr_index = 0;
    flink* cur_link = lfrnt->head;
    while(index + cur_link->lfr->size < fr_start) {
        index += cur_link->lfr->size;
        cur_link = cur_link->next;
    }
    fr_index = fr_start - index;
    index += fr_index;

    while(index < fr_end) {
        if (fr_index >= cur_link->lfr->size) {
            cur_link = cur_link->next;
            fr_index = 0;
        }
        
        fr_pair* cur_pair = (fr_pair*)get_item_frontier(cur_link->lfr, fr_index);
        num_expanded++;

        if (global_dists4->data[cur_pair->node_val] != -1) {
            free(cur_pair);
            fr_index++;
            index++;
            continue;
        }
 
        global_dists4->data[cur_pair->node_val] = cur_pair->dist;

        succs_to_fr4(out_fr, cur_pair);
        free(cur_pair);
        
        fr_index++;
        index++;
    }
    
    return out_fr;
}

void* bfs_worker_start4(void* arg) {
    job4 j = *((job4*)arg);
    free(arg);
    frontier* ret_fr = bfs_worker4(j.thread_num, j.num_threads);
    return (void*)ret_fr;
}

void run_bfs_workers4(int num_threads, linked_frnts* start_lfrnt) {
    linked_frnts* cur_lfrnt = start_lfrnt;
    while(1) {
        if (cur_lfrnt->tot_size <= 0) {
            free_lfrnts(cur_lfrnt);
            return;
        }

        pthread_t threads[num_threads];
        long partition_size = cur_lfrnt->tot_size / num_threads;

        for (int i = 0; i < num_threads; i++) {
            long start = i * partition_size;
            long end;
            if (i == num_threads) {
                end = cur_lfrnt->tot_size;
            } else {
                end = (i + 1) * partition_size;
            }
            job4* temp_job = malloc(sizeof(job4));
            temp_job->thread_num = i;
            temp_job->lfrnt = cur_lfrnt;
            temp_job->fr_start = start;
            temp_job->fr_end = end;
            temp_job->num_threads = num_threads;

            //printf("\nStarting thread %d\n", i);
            //printf("Frontier start: %ld\n", start);
            //printf("Frontier end:   %ld\n", end);

            pthread_t thread;
            int rv = pthread_create(&thread, 0, bfs_worker_start4, temp_job);
            assert(rv == 0);
            threads[i] = thread;
        }

        
        linked_frnts* new_lfrnt = make_lfrnts();
        for (int i = 0; i < num_threads; i++) {
            void* ret_val;
            int rv = pthread_join(threads[i], &ret_val);
            assert(rv == 0);
            frontier* thread_frnt = (frontier*)ret_val;
            add_frnt(new_lfrnt, thread_frnt);
        }
        free_lfrnts(cur_lfrnt);
        cur_lfrnt = new_lfrnt;
    }    
}

int par_bfs4(map_vec* map, long source, int num_threads, int print_output) {
    global_map4 = map;
    linked_frnts* init_lfrnt = make_lfrnts();
    frontier* init_fr = make_frontier();
    global_dists4 = make_explored();
    for (long i = 0; i < global_map4->size; i++) {
        push_explored(global_dists4, -1);
    }
    fr_pair* init_pair = new_pair(source, 0);
    global_dists4->data[source] = 0;
    succs_to_fr4(init_fr, init_pair);
    add_frnt(init_lfrnt, init_fr);

    run_bfs_workers4(num_threads, init_lfrnt);
    
    if (print_output) {
        for (long i = 0; i < global_dists4->size; i++) {
            printf("DIST FROM %ld TO %ld = %ld\n", source, i, global_dists4->data[i]);
        }
    }
    free(init_pair);
    free_explored(global_dists4);

    return 0;
}
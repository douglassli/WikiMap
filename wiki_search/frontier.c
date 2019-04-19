#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "frontier.h"

/*
    typedef struct frontier {
        long size;
        long cap;
        long* vals;
    } frontier;
*/

frontier* make_frontier() {
    frontier* fr = malloc(sizeof(frontier));
    fr->vals = malloc(2 * sizeof(long));
    fr->size = 0;
    fr->cap = 2;
    fr->offset = 0;
    return fr;
}

void remove_offset(frontier* fr) {
    void* new_data = malloc(fr->cap * sizeof(long));
    void* old_data = (void*)fr->vals + (fr->offset * sizeof(long));
    memcpy(new_data, old_data, fr->size * sizeof(long));
    free(fr->vals);
    fr->vals = (long*)new_data;
    fr->offset = 0;
}


void push_frontier(frontier* fr, long val) {
    //printf("\npush_frontier START\n");
    //printf("size = %ld\n", fr->size);
    //printf("cap = %ld\n", fr->cap);
    //printf("offset = %ld\n", fr->offset);
    //printf("PUSH VAL: %ld\n", val);
    if (fr->size >= fr->cap) {
        if (fr->offset > 0) {
            fr->cap = (fr->offset + fr->cap) * 2;
            //void* new_data = malloc(fr->cap * sizeof(long));
            //void* old_data = (void*)fr->vals + (fr->offset * sizeof(long));
            //memcpy(new_data, old_data, fr->size * sizeof(long));
            //free(fr->vals);
            //fr->vals = (long*)new_data;
            //fr->offset = 0;
            remove_offset(fr);
        } else {
            fr->cap *= 2;
            fr->vals = realloc(fr->vals, fr->cap * sizeof(long));
        }
    } 
    fr->vals[fr->size + fr->offset] = val;
    fr->size += 1;
    //printf("push_frontier END\n");
    //printf("size = %ld\n", fr->size);
    //printf("cap = %ld\n", fr->cap);
    //printf("offset = %ld\n", fr->offset);
}

void free_frontier(frontier* fr) {
    free(fr->vals);
    free(fr);
}

long pop_last_frontier(frontier* fr) {
    if (fr->size <= 0) {
        return -1;
    }


    long return_val = fr->vals[fr->size + fr->offset];
    fr->size -= 1;

    if (fr->size < fr->cap / 2) {
        fr->cap /= 2;
        fr->vals = realloc(fr->vals, fr->cap * sizeof(long));
    }
    
    return return_val;
}

long pop_first_frontier(frontier* fr) {
    //printf("\npop_first START\n");
    //printf("size = %ld\n", fr->size);
    //printf("cap = %ld\n", fr->cap);
    //printf("offset = %ld\n", fr->offset);
    if (fr->size <= 0) {
        return -1;
    }

    long return_val = fr->vals[fr->offset];
    fr->size -= 1;
    fr->cap -= 1;
    fr->offset++;
    //printf("pop_first FINISH\n");
    //printf("size = %ld\n", fr->size);
    //printf("cap = %ld\n", fr->cap);
    //printf("offset = %ld\n", fr->offset);
    //printf("POPFIRST RETURN VAL: %ld\n", return_val);
    return return_val;
}

void print_frontier(frontier* fr) {
    printf("FRONTIER CONTENTS: ");
    for (long i = fr->offset; i < fr->size + fr->offset; i++) {
        printf("%ld ", fr->vals[i]);
    }
    printf("\n");
}

frontier* merge_frontiers(frontier* fr1, fronter* fr2) {
    frontier* new_fr = malloc(sizeof(frontier));
    new_fr->vals = malloc((fr1->cap + fr2->cap) * sizeof(long));
    void* fr1_data = (void*)fr1->vals + (fr1->offset * sizeof(long));
    void* fr2_data = (void*)fr2->vals + (fr2->offset * sizeof(long));
    memcpy((void*)new_fr->vals, fr1_data, fr1->size * sizeof(long));
    memcpy((void*)new_fr->vals + (fr1->size * sizeof(long)), fr2_data, fr2->size * sizeof(long));
    new_fr->size = fr1->size + fr2->size;
    new_fr->cap = fr1->cap + fr2->cap;
    new_fr->offset = 0;
    free_frontier(fr1);
    free_frontier(fr2);
    return new_fr; 
}
/*
int main() {
    frontier* frnt = make_frontier();
    for (int n = 0; n < 5; n++) {
        for (int i = 10 * n; i < (10 * n) + 10; i++) {
            push_frontier(frnt, i);
            printf("FRNT size: %ld cap: %ld offset: %ld\n", frnt->size, frnt->cap, frnt->offset);
            print_frontier(frnt);
        }
        print_frontier(frnt);
        for (int i = 0; i < 5; i++) {
            long val = pop_first_frontier(frnt);
            printf("POPPED %ld\n", val);
            print_frontier(frnt);
        }
        print_frontier(frnt);
    }
    
    for (int i = 0; i < 10; i++) {
        push_frontier(frnt, i);
    }
    print_frontier(frnt);
    for (int i = 0; i < 5; i++) {
        long val = pop_first_frontier(frnt);
        printf("POPPED %ld\n", val);
    }
    print_frontier(frnt);
    for (int i = 10; i < 20; i++) {
        push_frontier(frnt, i);
    }
    print_frontier(frnt);
    for (int i = 0; i < 5; i++) {
        long val = pop_first_frontier(frnt);
        printf("POPPED %ld\n", val);
    }
    print_frontier(frnt);
    
    return 0;
}*/

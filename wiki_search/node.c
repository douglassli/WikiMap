#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
/*
    typedef struct node {
        long node;
        long size;
        long cap;
        long* data;
    } node;
*/


node* make_node(long node_val) {
    node* xs = malloc(sizeof(node));
    xs->node = node_val;
    xs->data = malloc(2 * sizeof(long));
    xs->size = 0;
    xs->cap = 2;
    return xs;
}

node* copy_node(node* xs) {
    node* new_node = malloc(sizeof(node));
    new_node->node = xs->node;
    new_node->size = xs->size;
    new_node->cap = xs->cap;
    new_node->data = malloc(xs->cap * sizeof(long));
    memcpy(new_node->data, xs->data, xs->cap * sizeof(long));
    return new_node;
}

void push_node(node* xs, long succ) {
    if (xs->size >= xs->cap) {
        xs->cap *= 2;
        xs->data = realloc(xs->data, xs->cap * sizeof(long));
    }

    xs->data[xs->size] = succ;
    xs->size += 1;
}

void free_node(node* xs) {
    free(xs->data);
    free(xs);
}

void print_node(node* xs) {
    printf("Node value: %ld\nEdges: ", xs->node);
    for (long i = 0; i < xs->size; ++i) {
        printf("%ld ", xs->data[i]);
    }
    printf("\n");
}

void print_edges(node* xs) {
    for (long i = 0; i < xs->size; ++i) {
        printf("%ld ", xs->data[i]);
        if (i != xs->size - 1) {
            printf("-> ");
        }
    }
    printf("\n");
}

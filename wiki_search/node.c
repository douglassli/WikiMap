#include <stdlib.h>
#include <stdio.h>

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

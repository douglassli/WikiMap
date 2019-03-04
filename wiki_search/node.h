#ifndef NODE_H
#define NODE_H

typedef struct node {
    long node;
    long size;
    long cap;
    long* data;
} node;

node* make_node(long node_val);
void node_push(node* xs, long succ);
void free_node(node* xs);
void print_node(node* xs);

#endif

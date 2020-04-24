#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "tree.h"

#define MEM_ALLOC_ERR 11

Node* create_node(double value, int nr_children) {
    Node* node = (Node*) malloc( sizeof( struct node ) + nr_children * sizeof( struct node* ) );
    if (node == NULL){
      errx(MEM_ALLOC_ERR, "%s:%d (%s): can't allocate node array of size %d.\n",
	   __FILE__, __LINE__, __func__, nr_children);
    }
    
    node->value = value;
    node->nr_children = nr_children;
    return node;
}

double get_value( const Node* node ) {
    return node->value;
}

void set_value( Node* node, double value ) {
    node->value = value;
}

int get_nr_children( const Node* node ) {
    return node->nr_children;
}

Node* get_child(const Node* node, int child_nr) {
    return node->child[child_nr];
}

void set_child( Node* node, int child_nr, Node* child ) {
    node->child[child_nr] = child;
}

void show_r( const Node* node, char prefix[]) {
    printf("%s%.3f, children: %d\n", prefix, node->value,
           get_nr_children(node));
    char new_prefix[10];
    sprintf(new_prefix, "%s  ", prefix);
    for (int child_nr = 0; child_nr < get_nr_children(node); child_nr++)
      show_r( (const Node*) get_child(node, child_nr), new_prefix);
}

void show( const Node* node) {
    show_r(node, "");
}

void visit( Node* node, double (*transf)(double) ) {
    node->value = transf(node->value);
    for (int child_nr = 0; child_nr < get_nr_children( (const Node*) node); child_nr++)
        visit(get_child(node, child_nr), transf);
}

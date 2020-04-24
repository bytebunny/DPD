/*!
  \file tree.c
  \author Rostyslav Skrypnyk
  \brief Implementation of the N-ary tree functionality.
*/

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "tree.h"

#define MEM_ALLOC_ERR 11

/*!
  \brief Allocates memory for a node and sets its parameters.
  \param value Value that the node holds.
  \param nr_children Number of children that the node has.
  \return Address where the `node` structure is stored.
*/
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

/*!
  \brief Fetches the value of a node.
  \param node Address of the node to fetch the value from.
  \return Value of the node.
*/
double get_value( const Node* node ) {
    return node->value;
}

/*!
  \brief Stores the value at a node.
  \param node Address of the node to store the value in.
  \param value Value to be stored.
  \return Void.
*/
void set_value( Node* node, double value ) {
    node->value = value;
}

/*!
  \brief Fetches the number of children a node has.
  \param node Address of the node to count children for.
  \return Number of children.
*/
int get_nr_children( const Node* node ) {
    return node->nr_children;
}

/*!
  \brief Fetches a child of a node.
  \param node Address of the node to fetch a child of.
  \param child_nr Child number.
  \return Address of the child node.
*/
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

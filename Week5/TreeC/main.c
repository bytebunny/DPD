#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

double add_one(double value);

int main() {
    Node *root = create_node(1.0, 3);
    Node *root_child_0, *root_child_1, *root_child_2,
      *node_child_0, *node_child_1, *node_child;
    { 
      root_child_0 = create_node(2.0, 2);
      set_child( root, 0, root_child_0 );
      {
	Node *node_p = get_child( root, 0 );
	
	node_child_0 = create_node(3.0, 0);
	set_child( node_p, 0, node_child_0 );
	
	node_child_1 = create_node(4.0, 0);
	set_child( node_p, 1, node_child_1 );
      }
      root_child_1 = create_node(5.0, 0);
      set_child( root, 1, root_child_1 );
      {
	// no children
      }
      root_child_2 = create_node(6.0, 1);
      set_child( root, 2, root_child_2 );
      {
	Node *node_p = get_child( root, 2 );
	node_child = create_node(7.0, 0);
	set_child( node_p, 0, node_child );
      }
    }

    printf("tree:\n");
    show( root );
    visit( root, add_one);
    printf("transformed tree:\n");
    show( root );

    free(node_child);
    free(root_child_2);
    free(root_child_1);
    free(node_child_1);
    free(node_child_0);
    free(root_child_0);
    free(root);
    return 0;
}

double add_one(double value) {
    return value + 1.0;
}

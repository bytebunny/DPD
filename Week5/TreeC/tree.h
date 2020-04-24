#ifndef TREE_HDR
#define TREE_HDR

/*!
  \brief Data structure for a node in the tree.
*/
struct node {
    double value;  /**< Value at the node.*/
    int nr_children; /**< Number of children the node has.*/
  
    // flexible array member (FAM) must be the last member of struct. FAM occupies 0 byte in memory, needs malloc.
    struct node *child[]; /**< Flexible array member to hold addresses of children of the node. */
};

typedef struct node Node;

Node* create_node(double value, int nr_children);
double get_value( const Node* node );
void set_vallue( Node *node, double value );
int get_nr_children( const Node* node);
Node* get_child( const Node* node, int child_nr );
void set_child( Node* node, int child_nr, Node* child );
void show( const Node* node);
void visit( Node* node, double (*transf)(double) );

#endif

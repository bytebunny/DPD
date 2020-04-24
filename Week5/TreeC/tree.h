#ifndef TREE_HDR
#define TREE_HDR

struct node {
    double value;
    int nr_children;
    // flexible array member (FAM) must be the last member of struct.
    struct node *child[]; // FAM --> occupies 0 byte in memory, needs malloc.
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

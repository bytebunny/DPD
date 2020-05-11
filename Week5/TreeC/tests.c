#include <CUnit/Basic.h>

#include "tree.h"
#include "tests.h"

void test_node_create(void)
{
    Node* node = create_node( 1.0, 0 );
    CU_ASSERT_TRUE( node->value == 1.0 );
    CU_ASSERT_TRUE( node->nr_children == 0 );
}

void test_get_value(void)
{
    Node* node = create_node( 1.0, 0 );
    CU_ASSERT_TRUE( node->value == get_value( (const Node*) node ) );
}

void test_get_nr_children(void)
{
    Node* node = create_node( 1.0, 0 );
    CU_ASSERT_TRUE( node->nr_children == get_nr_children( (const Node*) node ) );
}

void test_set_value(void)
{
    Node* node = create_node( 1.0, 0 );
    set_value( node, 0.0 );
    CU_ASSERT_TRUE( node->value == 0.0 );
}

void test_set_child(void)
{
    Node* root = create_node( 1.0, 1 );
    Node* child = create_node( 1.0, 0 );
    set_child( root, 0, child );
    CU_ASSERT_TRUE( root->child[0] == child );
}

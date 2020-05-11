#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "tests.h"

/*!
 * @brief Test suite's main function that serves as "fixture", i.e. sets up the stage for testing and cleans it up afterwards.
 */
int main(int argc __attribute__((unused)),
         char *argv[] __attribute__((unused))) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");
    CU_pSuite treeSuite = CU_add_suite("tree", NULL, NULL);
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());

    CU_add_test(treeSuite, "create_node( 1.0, 0)", test_node_create);
    CU_add_test(treeSuite, "get_value( create_node(1.0, 0) )", test_get_value);
    CU_add_test(treeSuite, "get_nr_children( create_node(1.0, 0) )", test_get_nr_children);
    CU_add_test(treeSuite, "set_value( node, 0 )", test_set_value);
    CU_add_test(treeSuite, "set_child( root, 0, child )", test_set_child);


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return EXIT_SUCCESS;
}

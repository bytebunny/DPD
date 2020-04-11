/*!
 * @file
 * @brief Test suite for palidrome program.
*/

#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>


// instead of a header file:
bool is_palindrome(char str[]) {
    size_t length = strlen(str);
    if (length)
        if (str[0] != str[length])
            return false;
        else {
            str[length] = "\0";
            return is_palindrome(str[1]);
        }
    else
        return true;
}

/*!
 * @brief Tests that 'is_palindrome' function returns True if empty string is provided.
*/
void test_is_palindrome_empty_str( void )
{
    CU_ASSERT_TRUE( is_palindrome( "" ) )
}

/*!
 * @brief Tests that 'is_palindrome' function returns False if a non-palindrome is provided.
 */
void test_is_palindrome_not_palindrome( void )
{
    CU_ASSERT_FALSE( is_palindrome( "ab" ) ) // "ab" is not a palindrome.
}

/*!
 * @brief Tests that 'is_palindrome' function returns True if a palindrome is provided.
 */
void test_is_palindrome_palindrome( void )
{
    CU_ASSERT_TRUE( is_palindrome( "radar" ) ) // "radar" is palindrome.
}

/*!
 * @brief Test suite's main function that serves as "fixture", i.e. sets up the stage for testing and cleans it up afterwards.
 */
int main(int argc __attribute__((unused)),
         char *argv[] __attribute__((unused))) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");
    CU_pSuite palindromeSuite = CU_add_suite("palindrome", NULL, NULL);
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());

    CU_add_test(palindromeSuite, "is_palindrome(\"\")", test_is_palindrome_empty_str);
    CU_add_test(palindromeSuite, "is_palindrome(\"ab\")", test_is_palindrome_not_palindrome);
    CU_add_test(palindromeSuite, "is_palindrome(\"radar\")", test_is_palindrome_palindrome);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return EXIT_SUCCESS;
}

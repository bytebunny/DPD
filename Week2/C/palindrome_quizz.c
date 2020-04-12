#include <stdbool.h>
#include <string.h>

bool is_palindrome(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n/2; i++)
        if (str[i] != str[n - i - 1])
            return false;
    return true;
}

int main()
{
    is_palindrome("abcba");
    return 0;
}

#include "syscall.h"
#include "test_utility.h"
#define val1 'a'
#define val2 'b'
#define val3 'c'
#define val4 'd'

int main()
{
    char a = GetChar();
    char b = GetChar();
    char c = GetChar();
    char d = GetChar();
    ASSERT_TEST(a==val1);
    ASSERT_TEST(b == val2);
    ASSERT_TEST(c==val3);
    ASSERT_TEST(d==val4);
    Exit(0);
}

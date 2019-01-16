#include "syscall.h"
#include "test_utility.h"
#define val 'a'

int main()
{
    char c = GetChar();
    ASSERT_TEST(c==val);
    Exit(0);
}

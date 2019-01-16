#include "syscall.h"
#include "test_utility.h"
#define MYEOF -1

int main()
{
    char a = GetChar();
    ASSERT_TEST(a==MYEOF);  //EOF
    Exit(0);
}

#include "syscall.h"
#include "test_utility.h"

int main()
{
    char s[1];
    GetString(s,1);
    ASSERT_TEST(s[0]=='\0');
}
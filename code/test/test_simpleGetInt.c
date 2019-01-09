#include "syscall.h"
#include "test_utility.h"

int main()
{
    int val = GetInt();
    ASSERT_TEST(val == 42);
}
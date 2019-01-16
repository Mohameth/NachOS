#include "syscall.h"
#include "test_utility.h"
int main()
{
    int res0 = Create("file1",10);
    int res1 = Create("file2",10);
    int res2 = Create("file3",10);
    int res3 = Create("file4",10);
    int res4 = Create("file5",10);
    int res5 = Create("file6",10);
    int res6 = Create("file7",10);
    int res7 = Create("file8",10);

    ASSERT_TEST(res0 == 1); //create success
    ASSERT_TEST(res1 == 1); 
    ASSERT_TEST(res2 == 1); 
    ASSERT_TEST(res3 == 1); 
    ASSERT_TEST(res4 == 1); 
    ASSERT_TEST(res5 == 1); 
    ASSERT_TEST(res6 == 1); 
    ASSERT_TEST(res7 == 1); 
    Exit(0);
}

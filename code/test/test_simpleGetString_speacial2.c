#include "syscall.h"
#include "test_utility.h"

int main()
{
    //input a puis ENTRER
    char s[4] = "wxyz";
    GetString(s,4);
    ASSERT_TEST(s[0]=='a');
    ASSERT_TEST(s[1]=='\n');
    ASSERT_TEST(s[2]=='\0');
    ASSERT_TEST(s[3]=='\0');
    Exit(0);
}

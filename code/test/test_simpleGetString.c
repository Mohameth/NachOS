#include "syscall.h"
#include "test_utility.h"

int main()
{
    char s[30];
    GetString(s,30);
    PutString(s);
    // ASSERT_TEST(s[0]=='a');
    // ASSERT_TEST(s[1]=='b');
    // ASSERT_TEST(s[2]=='\0'); //d'après la documentation le dernier caractère est remplacé par \0
    Exit(0);
}

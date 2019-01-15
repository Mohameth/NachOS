#include "syscall.h"
#include "test_utility.h"
int main()
{
    char *filename = "file.txt";
    Create(filename,10);
    OpenFileId f1 = Open(filename);
    ASSERT_TEST(f1==0);
}
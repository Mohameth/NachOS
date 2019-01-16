#include "syscall.h"
#include "test_utility.h"
int main()
{
    int d1 = CreateRepository("dossier1");
    ASSERT_TEST(d1==1);
    PrintRepository();

    int d2 = ChangeRepository("dossier1");
    ASSERT_TEST(d2 == 1);

    int d3 = CreateRepository("subDir");
    PrintRepository();

    ASSERT_TEST(d3 == 1);

    int d4 = ChangeRepository("subDir");
    ASSERT_TEST(d4 == 1);

    int d5 = CreateRepository("subsubDir");
    PrintRepository();
    ASSERT_TEST(d5 == 1);

    int d6 = ChangeRepository("subsubDir");
    ASSERT_TEST(d6 == 1);

    int d7 = CreateRepository("subsubsub");
    int d8 = ChangeRepository("subsubsub");
    ASSERT_TEST(d8 == 1);

    int res0 = Create("file1",10);
    ASSERT_TEST(res0 == 1);

    PrintRepository();
    ASSERT_TEST(d7 == 1);
}
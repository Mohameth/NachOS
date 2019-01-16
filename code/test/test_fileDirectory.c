#include "syscall.h"
#include "test_utility.h"
int main()
{

    int d1 = CreateRepository("dossier1");
    int d2 = CreateRepository("dossier2");
    int d3 = CreateRepository("dossier3");
    int d4 = CreateRepository("dossier4");
    int d5 = CreateRepository("dossier5");
    int d6 = CreateRepository("dossier6");
    int d7 = CreateRepository("dossier7");
    int d8 = CreateRepository("dossier8");


    ASSERT_TEST(d1==1);
    ASSERT_TEST(d2==1);
    ASSERT_TEST(d3==1);
    ASSERT_TEST(d4==1);
    ASSERT_TEST(d5==1);
    ASSERT_TEST(d6==1);
    ASSERT_TEST(d7==1);
    ASSERT_TEST(d8==1);
    
    PrintRepository();

    
}
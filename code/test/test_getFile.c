#include "syscall.h"
#include "test_utility.h"

int main()
{
    //le serveur est -m 1
    int to = 1;
    int res = ClientGet(to ,"file.txt");
    ASSERT_TEST(res !=-1);
    OpenFileId f1 = Open("file.txt");
    ASSERT_TEST(f1 == 0);

    char bufReadF1[3];
    int nbLu = Read(bufReadF1, 3, f1);

    ASSERT_TEST(nbLu == 3);

    ASSERT_TEST(bufReadF1[0] == 'a');
    ASSERT_TEST(bufReadF1[1] == 'b');
    ASSERT_TEST(bufReadF1[2] == 'c');
}
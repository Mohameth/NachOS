#include "syscall.h"
#include "test_utility.h"

int main()
{
    Serveur();

    OpenFileId f1 = Open("file.txt");

    char bufReadF1[3];
    int nbLu = Read(bufReadF1, 3, f1);

    ASSERT_TEST(nbLu == 3);

    ASSERT_TEST(bufReadF1[0] == 'a');
    ASSERT_TEST(bufReadF1[1] == 'b');
    ASSERT_TEST(bufReadF1[2] == 'c');

    PrintRepository();
    Close(f1);
    Exit(0);
}
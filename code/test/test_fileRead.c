#include "syscall.h"
#include "test_utility.h"
int main()
{

    //Lire plus de donnée que dans le fichier

    int res0 = Create("file1",10);
    ASSERT_TEST(res0 == 1);

    OpenFileId f1 = Open("file1");
    ASSERT_TEST(f1 == 0);

    char *buf = "abcdefg\0";
    int n = Write(buf, 5, f1); //ecrit Texte dans le fichier f1
    Seek(0, f1);
    ASSERT_TEST(n == 5);

    char bufReadF1[15];
    int nbLu = Read(bufReadF1, 12, f1);
    PutInt(nbLu);
    ASSERT_TEST(nbLu == 10);

    ASSERT_TEST(bufReadF1[0] == 'a');
    ASSERT_TEST(bufReadF1[1] == 'b');
    ASSERT_TEST(bufReadF1[2] == 'c');
    ASSERT_TEST(bufReadF1[3] == 'd');
    ASSERT_TEST(bufReadF1[4] == 'e');
    ASSERT_TEST(bufReadF1[5] != 'f');
    ASSERT_TEST(bufReadF1[6] != 'g');
    Close(f1);
}
#include "syscall.h"
#include "test_utility.h"
int main()
{
    //Ce test fonctionne pour un disque formaté
    char *filename = "file.txt";
    int res = Create(filename,10);
    OpenFileId f1 = Open(filename);
    ASSERT_TEST(f1==0);
    ASSERT_TEST(res == 1); //creation success

    char* buf = "abcdefg";
    int n = Write(buf, 5, f1); //ecrit Texte dans le fichier f1
    Seek(0,f1);
    ASSERT_TEST(n==5);

    char bufReadF1[3];
    int nbLu = Read(bufReadF1, 3, f1);

    ASSERT_TEST(nbLu == 3);

    ASSERT_TEST(bufReadF1[0]=='a');
    ASSERT_TEST(bufReadF1[1]=='b');
    ASSERT_TEST(bufReadF1[2]=='c');
}
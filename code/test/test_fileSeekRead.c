#include "syscall.h"
#include "test_utility.h"

char readPos(int val, OpenFileId fid)
{
    char* bufReadF1 ="";
    Seek(val, fid);
    int nbLu = Read(bufReadF1, 1, fid);
    ASSERT_TEST(nbLu == 1);
    return bufReadF1[0];
}

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
    ASSERT_TEST(n==5);

    char bufRead = 'z';

    bufRead = readPos(0,f1);
    ASSERT_TEST(bufRead == 'a');

    bufRead = readPos(1,f1);
    ASSERT_TEST(bufRead == 'b');

    bufRead = readPos(2,f1);
    ASSERT_TEST(bufRead == 'c');
}
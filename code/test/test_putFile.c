#include "syscall.h"
#include "test_utility.h"

int main()
{
    char *filename = "file.txt";
    int c = Create(filename, 10);
    ASSERT_TEST(c == 1); //create success

    OpenFileId f1 = Open(filename);

    char *buf = "abcdefg";
    int n = Write(buf, 5, f1); //ecrit Texte dans le fichier f1
    ASSERT_TEST(n == 5);
    Close(f1);
    PrintRepository();

    //le serveur identifiant = 1
    int to = 1;
    int res = ClientPut(to ,filename);
    ASSERT_TEST(res == 0);
    Exit(0);
}
#include "syscall.h"
#include "test_utility.h"

int main()
{
    
    int res = Create("file.txt", 10);
    ASSERT_TEST(res == 1); //create success

    OpenFileId f1 = Open("file.txt");
    ASSERT_TEST(f1 == 0);

    char *buf = "abcdefg";
    int n = Write(buf, 5, f1); //ecrit Texte dans le fichier f1
    ASSERT_TEST(n == 5);
    
    Serveur();
}
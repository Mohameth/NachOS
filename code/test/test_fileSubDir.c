#include "syscall.h"
#include "test_utility.h"
int main()
{
    int d1 = CreateRepository("dossier1");
    ASSERT_TEST(d1==1);
    PutString("Répertoire dossier1 crée \n");

    int d2 = ChangeRepository("dossier1");
    ASSERT_TEST(d2 == 1);
    PutString("On rentre dans dossier1 puis on crée subDir\n");

    int d3 = CreateRepository("subDir");
    PutString("Affichage du contenu de dossier1 : \n");
    PrintRepository();

    ASSERT_TEST(d3 == 1);

    int d4 = ChangeRepository("subDir");
    ASSERT_TEST(d4 == 1);
    PutString("On rentre dans subDir \n");

    int d5 = CreateRepository("subsubDir");
    ASSERT_TEST(d5 == 1);

    int d6 = ChangeRepository("subsubDir");
    ASSERT_TEST(d6 == 1);

    int d7 = CreateRepository("subsubsub");
    int d8 = ChangeRepository("subsubsub");

    PutString("On crée et on va dans subsubDir/subsubsub/ crée \n\n");
    ASSERT_TEST(d7 == 1);
    ASSERT_TEST(d8 == 1);

    ASSERT_TEST(ChangeRepository("..") == 1);
    ASSERT_TEST(ChangeRepository("..") == 1);
    ASSERT_TEST(ChangeRepository("..") == 1);
    PutString("Retour au repertoire subDir et affichage du contenu :\n");
    PrintRepository();

    int res0 = Create("file1",10);
    ASSERT_TEST(res0 == 1);

    PrintRepository();
    Exit(0);
}

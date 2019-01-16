#include "syscall.h"
#include "test_utility.h"

#define ENT 42
#define CHARA 'A'

typedef struct{
    int entier;
    char c;
} maStructure;


void function(void * arg) {
    maStructure *s = (maStructure *)arg;
    ASSERT_TEST(s->entier == ENT);
    ASSERT_TEST(s->c == CHARA);
    PutInt(s->entier);
    PutChar(s->c);
    UserThreadExit();
}

int main(){
    maStructure a;
    a.c = CHARA;
    a.entier = ENT;

    int tid = UserThreadCreate(function,&a);
    ASSERT_TEST(tid == 0);
    UserThreadJoin(tid);
    return 0;
}
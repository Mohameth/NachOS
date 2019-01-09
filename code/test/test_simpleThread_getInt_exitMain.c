#include "syscall.h"
#include "test_utility.h"

typedef struct{
    int entier;
    char c;
} maStructure;


void function(void * arg) {
    int *s = (int *)arg;
    ASSERT_TEST(*s == 42);

    int val = GetInt();
    ASSERT_TEST(val == 43);

    UserThreadExit();
}

int main(){
    int a = 42;
    int tid = UserThreadCreate(function,&a);
    ASSERT_TEST(tid == 0);
    //UserThreadJoin(tid);
    PutString("END MAIN\n");
    return 0;
}
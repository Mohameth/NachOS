#include "syscall.h"
#include "test_utility.h"

void functionTwo(void *arg)
{
    int *v = (int *)arg;
    ASSERT_TEST(*v == 42);
    PutInt(*v);
    PutInt(43);
}

void function(void * arg) {
    int * v = (int *) arg;
    ASSERT_TEST(*v==42);
    PutInt(*v);

    functionTwo(arg);
    UserThreadExit();
}

int main(){
    int arg = 42;
    int tid = UserThreadCreate(function,&arg);
    ASSERT_TEST(tid == 0);
    UserThreadJoin(tid);
    return 0;
}
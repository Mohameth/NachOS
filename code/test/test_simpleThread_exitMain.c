#include "syscall.h"
#include "test_utility.h"


void function(void * arg) {
    int * v = (int *) arg;
    ASSERT_TEST(*v==42);
    PutInt(*v);
    SLEEP_TEST(1000000);
    UserThreadExit();
}

int main(){
    int arg = 42;
    int tid = UserThreadCreate(function,&arg);
    ASSERT_TEST(tid == 0);
    //UserThreadJoin(tid);
    PutString("END MAIN\n");
    return 0;
}
#include "syscall.h"
#include "test_utility.h"
#define NB_THREAD_MAX 2

void function(void *arg)
{
    int *v = (int *)arg;
    PutInt(*v);
    ASSERT_TEST(*v == 42);
    UserThreadExit();
}

//Ce test n'est valable uniquement pour la création de deux thread a maximum
int main(){
    int arg = 42;
    for (int i = 0; i < NB_THREAD_MAX; i++){ //noter le NB_THREAD_MAX+1 dans la condtions
        UserThreadCreate(function, &arg);
    }

    for (int j = 0; j < NB_THREAD_MAX; j++){
        UserThreadJoin(j);
    }

    return 0;
}
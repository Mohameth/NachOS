#include "syscall.h"
#include "test_utility.h"


void function(void * arg) {
    OpenFileId id = Open("testF");
    ASSERT_TEST(id>=0);   
    UserThreadExit();
}

int main(){
    Create("testF", 15);
    int a;
    int tid1 = UserThreadCreate(function, &a);
    int tid2 = UserThreadCreate(function, &a);

    UserThreadJoin(tid1);
    UserThreadJoin(tid2);
    return 0;
}
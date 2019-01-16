#include "syscall.h"

void f(void * arg) {
    //PutString("thread N°");
    PutInt((int) arg);
    //PutChar('\n');
    while(1);
    UserThreadExit();
}

int main()
{
    int arg = 0;
    int i;
    for (i = 1; i<1000;i++) {
        UserThreadCreate(f,&arg);
    }
    while(1);
    Halt();
}

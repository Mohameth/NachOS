#include "syscall.h"

void f2() {
    PutString("azerty\n");
    UserThreadExit();
}

void f1() {
    PutString("QWERTY\n");
    UserThreadExit();
}

int main()
{
    UserThreadCreate(f1,0);
    UserThreadCreate(f2,0);
    PutString("POP");
    PutChar('\n');
    Exit(0);
}
#include "syscall.h"

void f2() {
    PutString("potimarron\n");
    UserThreadExit();
}

void f1() {
    PutString("TOPINAMBOUR\n");
    UserThreadExit();
}

int main()
{
    UserThreadCreate(f1,0);
    UserThreadCreate(f2,0);
    PutString("L0L");
    PutChar('\n');
    while(1);
    Halt();
}
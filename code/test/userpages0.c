#include "syscall.h"

void f2() {
    PutChar('c');
    PutChar('o');
    PutChar('u');
    PutChar('r');
    PutChar('g');
    PutChar('e');
    PutChar('t');
    PutChar('t');
    PutChar('e');
    PutChar('\n');
    UserThreadExit();
}

void f1() {
    PutChar('A');
    PutChar('N');
    PutChar('A');
    PutChar('N');
    PutChar('A');
    PutChar('S');
    PutChar('\n');
    UserThreadExit();
}

int main()
{
    UserThreadCreate(f1,0);
    UserThreadCreate(f2,0);
    PutChar('L');
    PutChar('\n');
    Exit(0);
}
#include "syscall.h"

void f2() {
    PutChar('0');
    PutChar('1');
    PutChar('2');
    PutChar('3');
    PutChar('4');
    PutChar('5');
    PutChar('6');
    PutChar('7');
    PutChar('8');
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
    PutString("PPP");
    PutChar('\n');
    Exit(0);
}
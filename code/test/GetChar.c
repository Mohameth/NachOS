#include "syscall.h"

void scan(int n) {
    int i;
    char c;
    for (i=0;i <n; i++) {
        //PutInt(i);
        c = GetChar();
        if (c == -1) //EOF
            Halt();
        PutChar(c);
    }
    PutChar('\n');
}

int main()
{
    scan(4);
    Halt();
}
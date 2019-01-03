
#include "syscall.h"

void print(void *c) 
{
    int i;
    char* x = (char *) c;
    for (i = 0; i < 4; i++) {
        PutChar(*x+i);
    }
    PutChar('\n');

    UserThreadExit();
}

void printa(void * a) {
    char * x = (char *) a;
    PutChar(*x);

    UserThreadExit();
}

int main() 
{
    PutChar('\n');

    char x = 'g';
    char y = 'a';
    int printId = UserThreadCreate(print, &x);
    UserThreadJoin(printId);

    int printAId = UserThreadCreate(printa, &y);
    UserThreadJoin(printAId);

    printId = UserThreadCreate(print, &x);
    UserThreadJoin(printId);

    printId = UserThreadCreate(print, &x);
    UserThreadJoin(printId);

    PutString("\nfin\n");
    
    return 1;
}
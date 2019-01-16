#include "syscall.h"
#include "test_utility.h"

int main()
{
    int to = 1;
    int data = 64;
    Send(to,(char *)&data);
}
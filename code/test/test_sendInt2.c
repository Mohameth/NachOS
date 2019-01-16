#include "syscall.h"
#include "test_utility.h"

int main()
{
    int to = 1;
    int data = 44;
    SendInt(to,data);
    Exit(0);
}

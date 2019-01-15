#include "syscall.h"
#include "test_utility.h"

int main()
{
    int to = 1;
    char * data = "a";
    Send(to,data);
}
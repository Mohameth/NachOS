#include "syscall.h"
#include "test_utility.h"

int main()
{
    char * data = "";
    Receive(data);
    PutString(data);
    ASSERT_TEST(data[0]=='a');
}
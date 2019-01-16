#ifndef TESTUTILITY_H
#define TESTUTILITY_H

#define ASSERT_TEST(condition)                                       \
    if (!(condition))                                                \
    {                                                                \
        PutString("==Condition ERROR, Assertion failed==\nLigne: "); \
        PutInt(__LINE__);                                            \
        PutString("\nFichier:");                                     \
        PutString(__FILE__);                                         \
        PutString("\n");                                     \
        Halt();                                                      \
    }

#define SLEEP_TEST(amount)             \
    PutString("\nSTART sleep\n");        \
    int i; 				\
    for (i = 0; i < (amount); i++) \
    {                                  \
    }                                  \
    PutString("END sleep\n");           \


#endif /* TESTUTILITY_H */

#ifndef USERTHREAD_H
#define USERTHREAD_H


typedef struct {
    int fonction;
    int arg;
} args;



extern int do_UserThreadCreate(int f, int arg);

extern void do_UserThreadExit();

#endif // USERTHREAD_H
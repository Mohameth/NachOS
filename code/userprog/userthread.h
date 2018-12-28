#ifndef USERTHREAD_H
#define USERTHREAD_H


typedef struct {
    int fonction;
    int arg;
    int sp;
} args;

int sp;

extern int do_UserThreadCreate(int f, int arg);

extern void do_UserThreadExit();

#endif // USERTHREAD_H
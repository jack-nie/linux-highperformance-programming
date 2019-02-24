#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
pthread_t ntid;

void 
printids(const char *s)
{
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void *
thr_fn(void *arg)
{
    printids("new thread: ");
    return ((void*)0);
}

void *
thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    return ((void*)1);
}

void *
thr_fn2(void *arg)
{
    printf("thread 2 returning\n");
    return ((void*)2);
}

int
main(int argc, char *argv[])
{
    int err;
    pthread_t tid1, tid2;
    void *tret;
    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0)
    {
        printf("cant't create thread: %d", err);
        exit(-1);
    }
    sleep(1);
    printids("main thread: ");
    pthread_create(&tid1, NULL, thr_fn1, NULL);
    pthread_create(&tid2, NULL, thr_fn2, NULL);
    pthread_join(tid1, &tret);
    printf("thread 1 exit code %ld\n", (long)tret);
    pthread_join(tid2, &tret);
    printf("thread 2 exit code %ld\n", (long)tret);
    return 0;
}
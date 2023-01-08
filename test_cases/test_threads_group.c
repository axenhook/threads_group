#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "threads_group.h"

static int g_exit_flag = 0;

void trigger_stop(int param)
{
    g_exit_flag = 1;
}

void *thread_test(void *arg, unsigned int thread_id)
{
    int cnt = 0;

    while (1)
    {
        if (g_exit_flag)
        {
            printf("The exit flag cached. [thread: %lu]\n", (unsigned long)pthread_self());
            break;
        }

        printf("The thread is running. [thread_id: %u, arg: %u, cnt: %d, thread: %lu]\n",
               thread_id, ((unsigned int *)arg)[thread_id], cnt++, (unsigned long)pthread_self());

        usleep(100 * 1000);
    }

    printf("The thread stopped. [thread_id: %u, cnt: %d, thread: %lu]\n",
           thread_id, cnt++, (unsigned long)pthread_self());

    return NULL;
}

int main(int argc, char *argv[])
{
    g_exit_flag = 0;

    signal(SIGINT, trigger_stop);
    // signal(SIGKILL, trigger_stop);
    signal(SIGTERM, trigger_stop);

    printf("start threads group. [thread: %lu]\n", (unsigned long)pthread_self());
    
    unsigned int arg[10] = {0};
    for (unsigned int i = 0; i < 10; i++)
    {
        arg[i] = i + 100;
    }

    void *group = threads_group_start(10, thread_test, arg);
    threads_group_wait_stop(group);

    printf("finish threads group. [thread: %lu]\n", (unsigned long)pthread_self());

    return 0;
}
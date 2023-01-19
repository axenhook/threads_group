/*
* from https://github.com/axenhook/threads_group.git
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "threads_group.h"

struct threads_group_param;

#if defined(WIN32)  // windows application

//#include <crtdbg.h>
#include <windows.h>

typedef HANDLE                      os_thread_t;

#define INVALID_TID                 NULL

#define atomic_inc(x)               InterlockedIncrement(x)
#define atomic_read(x)              (*(x))

#define os_sleep_ms(x)              Sleep(x)

static os_thread_t os_thread_create(void *(*func)(void *), void *para, const char *thread_name)
{
    return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, para, 0, NULL);
}

static void os_thread_destroy(os_thread_t tid, unsigned int force)
{
    if (force)
    {
        //TerminateThread(tid, 0);
        CloseHandle(tid);
    }
}

#else // linux user space application

#include <pthread.h>
#include <unistd.h>

typedef pthread_t                   os_thread_t;

#define INVALID_TID                 0

#define atomic_inc(x)               __sync_fetch_and_add(x, 1)
#define atomic_read(x)              (*(x))

#define os_sleep_ms(x)              usleep((x) * 1000)

static inline os_thread_t os_thread_create(void *(*func)(void *), void *para, const char *thread_name)
{
    unsigned int ret = 0;
    os_thread_t tid;

    ret = pthread_create(&tid, NULL, func, para);
    if (ret == 0)
    {
        return tid;
    }

    return INVALID_TID;
}

static inline void os_thread_destroy(os_thread_t tid, unsigned int force)
{
    if (force)
    {
        pthread_cancel(tid);
    }

    pthread_join(tid, NULL);

    return;
}

#endif

typedef struct threads_group
{
    unsigned int threads_num;      // number of threads wanted to be created
    unsigned int real_threads_num; // number of real threads created successfully
    unsigned int exit_threads_num; // number of thread exit
    os_thread_t *tids;

    struct threads_group_param *param;

    threads_group_func_t func;
    void *arg; // argument for func
} threads_group_t;

typedef struct threads_group_param
{
    threads_group_t *group;
    unsigned int thread_id;
} threads_group_param_t;

void threads_group_stop(void *threads_group, unsigned int is_force)
{
    threads_group_t *group = (threads_group_t *)threads_group;

    while (atomic_read(&group->exit_threads_num) < group->real_threads_num)
    {
        os_sleep_ms(500);
    }

    for (unsigned int i = 0; i < group->real_threads_num; i++)
    {
        os_thread_destroy(group->tids[i], is_force);
    }
}

void *thread_for_group(threads_group_param_t *arg)
{
    threads_group_t *group = arg->group;

    group->func(group->arg, arg->thread_id);
    atomic_inc(&group->exit_threads_num);

    return NULL;
}

void *threads_group_start(unsigned int threads_num, threads_group_func_t func, void *arg, const char *thread_name)
{
    os_thread_t *tids = (os_thread_t *)malloc(threads_num * sizeof(os_thread_t));
    if (tids == NULL)
    {
        return NULL;
    }

    memset(tids, 0, threads_num * sizeof(os_thread_t));

    threads_group_t *group = (threads_group_t *)malloc(sizeof(threads_group_t));
    if (group == NULL)
    {
        free(tids);
        return NULL;
    }

    memset(group, 0, sizeof(threads_group_t));
    group->tids = tids;
    group->func = func;
    group->arg = arg;
    group->threads_num = threads_num;

    threads_group_param_t *param = (threads_group_param_t *)malloc(threads_num * sizeof(threads_group_param_t));
    if (param == NULL)
    {
        return NULL;
    }

    for (unsigned int i = 0; i < threads_num; i++)
    {
        param[i].group = group;
        param[i].thread_id = i;
    }

    group->param = param;

    for (unsigned int i = 0; i < threads_num; i++)
    {
        os_thread_t tid = os_thread_create((void *(*)(void *))thread_for_group, &group->param[i], thread_name);
        if (tid == INVALID_TID)
        {
            break;
        }

        group->tids[i] = tid;
        group->real_threads_num++;
    }

    return group;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "threads_group.h"

struct threads_group_param;

typedef struct threads_group
{
    unsigned int threads_num;       // number of threads wanted to be created
    unsigned int real_threads_num;  // number of real threads created successfully
    pthread_t   *tids;

    struct threads_group_param *param;

    threads_group_func_t func;
    void *arg;  // argument for func
} threads_group_t;

typedef struct threads_group_param
{
    threads_group_t *group;
    unsigned int     thread_id;
} threads_group_param_t;


void threads_group_wait_stop(void *threads_group)
{
    threads_group_t *group = (threads_group_t *)threads_group;

	for (unsigned int i = 0; i < group->real_threads_num; i++)
	{
		pthread_join(group->tids[i], NULL);
	}
}

void *thread_for_group(threads_group_param_t *arg)
{
    threads_group_t *group = arg->group;

    group->func(group->arg, arg->thread_id);

    return NULL;
}

void *threads_group_start(unsigned int threads_num, threads_group_func_t func, void *arg)
{
    pthread_t *tids = (pthread_t *)malloc(threads_num * sizeof(pthread_t));
    if (tids == NULL)
    {
        return NULL;
    }

    memset(tids, 0, threads_num * sizeof(pthread_t));


    threads_group_t *group = malloc(sizeof(threads_group_t));
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
        pthread_t tid;
		int ret = pthread_create(&tid, NULL, (void * (*)(void *))thread_for_group, &group->param[i]);
		if (ret != 0)
		{
			break;
		}

        group->tids[i] = tid;
        group->real_threads_num++;
	}

    return group;
}


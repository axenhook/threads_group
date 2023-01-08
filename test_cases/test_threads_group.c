#include <assert.h>
#include <stdio.h>
#include <signal.h>

#include "../threads_group.h"

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
            printf("The exit flag cached. [thread: %lu]\n", os_get_thread_id());
            break;
        }

        printf("The thread is running. [thread_id: %u, arg: %u, cnt: %d, thread: %lu]\n",
               thread_id, ((unsigned int *)arg)[thread_id], cnt++, os_get_thread_id());

		os_sleep_ms(100);
    }

    printf("The thread stopped. [thread_id: %u, cnt: %d, thread: %lu]\n",
           thread_id, cnt++, os_get_thread_id());

    return NULL;
}

int main(int argc, char *argv[])
{
    g_exit_flag = 0;

    signal(SIGINT, trigger_stop);
    // signal(SIGKILL, trigger_stop);
    signal(SIGTERM, trigger_stop);

    printf("test threads group start. [thread: %lu]\n", os_get_thread_id());
    
    unsigned int arg[10] = {0};
    for (unsigned int i = 0; i < 10; i++)
    {
        arg[i] = i + 100;
    }

    void *group = threads_group_start(10, thread_test, arg, "test_threads_group");
	os_sleep_ms(5*1000);
	threads_group_stop(group, 0);
	//os_sleep_ms(5*1000);

    printf("test threads group finished. [thread: %lu]\n", os_get_thread_id());

	//system("pause");

    return 0;
}
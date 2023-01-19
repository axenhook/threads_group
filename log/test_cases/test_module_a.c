/*
 * from https://github.com/axenhook/log.git
 */

#include <assert.h>
#include <stdio.h>
#include <signal.h>

#include "../log.h"
#include "threads_group.h"

#ifdef WIN32 // windows application

#include <windows.h>

#define os_get_thread_id() ((unsigned long)GetCurrentThreadId())
#define os_sleep_ms(x) Sleep(x)
#define pause() system("pause");

#else // linux user space application

#include <unistd.h>
#include <pthread.h>

#define os_get_thread_id() ((unsigned long)pthread_self())
#define os_sleep_ms(x) usleep((x)*1000)

#endif

MODULE_ID(18);

extern int g_exit_flag;

static void print_log(int level, unsigned int thread_id, unsigned int cnt)
{
    log_set_level(level);

    log_debug("Test very1 very2 very3 very4 very5 very6 very7 very8 very9 very10 very11 very12 very13 very14 very15 very16 long %d, cnt %d\n", thread_id, cnt);
    log_info("Test very1 very2 very3 very4 very5 very6 very7 very8 very9 very10 very11 very12 very13 very14 very15 very16 long %d, cnt %d\n", thread_id, cnt);
    log_warn("Test very1 very2 very3 very4 very5 very6 very7 very8 very9 very10 very11 very12 very13 very14 very15 very16 long %d, cnt %d\n", thread_id, cnt);
    log_error("Test very1 very2 very3 very4 very5 very6 very7 very8 very9 very10 very11 very12 very13 very14 very15 very16 long %d, cnt %d\n", thread_id, cnt);
    log_emerg("Test very1 very2 very3 very4 very5 very6 very7 very8 very9 very10 very11 very12 very13 very14 very15 very16 long %d, cnt %d\n", thread_id, cnt);
    log_event("Test very1 very2 very3 very4 very5 very6 very7 very8 very9 very10 very11 very12 very13 very14 very15 very16 long %d, cnt %d\n", thread_id, cnt);
}

static void *thread_test(void *arg, unsigned int thread_id)
{
    unsigned int i = 0;

    while (1)
    {
        if (g_exit_flag)
        {
            break;
        }

        print_log(0, thread_id, i++);
        print_log(1, thread_id, i++);
        print_log(2, thread_id, i++);
        print_log(3, thread_id, i++);
        print_log(4, thread_id, i++);
        print_log(5, thread_id, i++);
        print_log(6, thread_id, i++);

        // usleep(100 * 1000);
    }

    printf("The thread stopped. [thread_id: %u, thread: %lu]\n",
           thread_id, os_get_thread_id());

    return NULL;
}

void *start_threads_group_module_a(void)
{
    log_set_name("test_module_a");
    return threads_group_start(20, thread_test, NULL, "test_module_a");
}

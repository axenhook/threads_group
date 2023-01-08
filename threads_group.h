/*
* from https://github.com/axenhook/threads_group.git
*/

#ifndef __THREADS_GROUP_H__
#define __THREADS_GROUP_H__

#ifdef WIN32  // windows application

#include <windows.h>

#define os_get_thread_id()               ((unsigned long)GetCurrentThreadId())
#define os_sleep_ms(x)                   Sleep(x)

#else // linux user space application

#include <unistd.h>
#include <pthread.h>

#define os_get_thread_id()               ((unsigned long)pthread_self())
#define os_sleep_ms(x)                   Sleep(x) // usleep((x) * 1000)

#endif

#ifdef __cplusplus
extern "C"
{
#endif /* End of __cplusplus */

typedef void *(*threads_group_func_t)(void *arg, unsigned int thread_id);

void *threads_group_start(unsigned int threads_num, threads_group_func_t func, void *arg, const char *thread_name);
void threads_group_stop(void *threads_group, unsigned int is_force);

#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif /* __THREADS_GROUP_H__ */
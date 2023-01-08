#ifndef __THREADS_GROUP_H__
#define __THREADS_GROUP_H__

#ifdef __cplusplus
extern "C"
{
#endif /* End of __cplusplus */

typedef void *(*threads_group_func_t)(void *arg, unsigned int thread_id);

void *threads_group_start(unsigned int threads_num, threads_group_func_t func, void *arg);
void threads_group_wait_stop(void *threads_group);

#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif /* __THREADS_GROUP_H__ */
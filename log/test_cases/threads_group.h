/*
* from https://github.com/axenhook/threads_group.git
*/

#ifndef __THREADS_GROUP_H__
#define __THREADS_GROUP_H__

#ifdef __cplusplus
extern "C"
{
#endif /* End of __cplusplus */

// @thread_id is 0,1,2...,@threads_num-1, 
// the @threads_num and @arg is assigned in API threads_group_start
typedef void *(*threads_group_func_t)(void *arg, unsigned int thread_id);

void *threads_group_start(unsigned int threads_num, threads_group_func_t func, void *arg, const char *thread_name);
void threads_group_stop(void *threads_group, unsigned int is_force);

#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif /* __THREADS_GROUP_H__ */

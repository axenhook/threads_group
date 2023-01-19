/*
* from https://github.com/axenhook/log.git
*/


#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

void set_module_level(unsigned int mid, int level);
int get_module_level(unsigned int mid);
void set_module_name(unsigned int mid, const char *name);
char *get_module_name(unsigned int mid);
void log_trace(unsigned int mid, unsigned char level, const char *fmt, ...);

//============================================================================
// external API functions
//============================================================================
// the value define of @mode in log_init function
#define LOG_TO_NULL        0x00
#define LOG_TO_FILE        0x01
#define LOG_TO_SCREEN      0x02
#define LOG_TO_SCNFILE     (LOG_TO_FILE | LOG_TO_SCREEN)

// @mode is defined by LOG_TO_xxx macros
int log_init(const char *log_name, unsigned int mode, unsigned int max_lines);
void log_destroy(void);

#define MODULE_ID(mid) static unsigned int g_mid = (mid)

#define log_set_level(level) set_module_level(g_mid, level)
#define log_get_level()      get_module_level(g_mid)
#define log_set_name(name)   set_module_name(g_mid, name)
#define log_get_name()       get_module_name(g_mid)

#if 1

#define log_debug(fmt, ...)                                              \
    log_trace(g_mid, 5, "[DEBUG][%s:%s:%s:%d]: " fmt, log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_info(fmt, ...)                                               \
    log_trace(g_mid, 4, "[INFO ][%s:%s:%s:%d]: " fmt, log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_warn(fmt, ...)                                               \
    log_trace(g_mid, 3, "[WARN ][%s:%s:%s:%d]: " fmt, log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_error(fmt, ...)                                              \
    log_trace(g_mid, 2, "[ERROR][%s:%s:%s:%d]: " fmt, log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_emerg(fmt, ...)                                              \
    log_trace(g_mid, 1, "[EMERG][%s:%s:%s:%d]: " fmt, log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_event(fmt, ...)                                              \
    log_trace(g_mid, 0, "[EVENT][%s:%s:%s:%d]: " fmt, log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#else

#define log_debug(fmt, ...)                                                                                        \
    log_trace(g_mid, 5, "[DEBUG][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_info(fmt, ...)                                                                                         \
    log_trace(g_mid, 4, "[INFO ][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_warn(fmt, ...)                                                                                         \
    log_trace(g_mid, 3, "[WARN ][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_error(fmt, ...)                                                                                        \
    log_trace(g_mid, 2, "[ERROR][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_emerg(fmt, ...)                                                                                        \
    log_trace(g_mid, 1, "[EMERG][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define log_event(fmt, ...)                                                                                        \
    log_trace(g_mid, 0, "[EVENT][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), log_get_name(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif

#endif

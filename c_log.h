#ifndef C_LOG
#define C_LOG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdarg.h>

#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <pthread.h>

typedef enum{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL,
    NO_LOG
} log_level;

void init_logging(int log_fd,bool log_to_stdout,log_level level);
void log_message(log_level level, const char *file, int line, const char *fmt, ...);

#define log(level, ...) \
    log_message(level, __FILE__, __LINE__, __VA_ARGS__)

#define log_debug(...) log(LOG_DEBUG, __VA_ARGS__)
#define log_info(...) log(LOG_INFO, __VA_ARGS__)
#define log_warning(...) log(LOG_WARNING, __VA_ARGS__)
#define log_error(...) log(LOG_ERROR, __VA_ARGS__)
#define log_critical(...) log(LOG_CRITICAL, __VA_ARGS__)

#endif
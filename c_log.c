#include "c_log.h"

#define LOG_LEVEL_STR(level) \
    (level == LOG_DEBUG ? "DEBUG" : \
    (level == LOG_INFO ? "INFO" : \
    (level == LOG_WARNING ? "WARN" : \
    (level == LOG_ERROR ? "ERROR" : \
    (level == LOG_CRITICAL ? "CRITICAL" : "UNKNOWN")))))

typedef struct {
    int log_fd;
    bool log_to_stdout;
    log_level level;
    bool log_time;
    pthread_mutex_t lock;
    bool sync_all;
    bool initialized;
} log_settings;

static log_settings settings = {-1, false,NO_LOG, true,PTHREAD_MUTEX_INITIALIZER,false,false};

void init_logging(int log_fd,bool log_to_stdout,log_level level,bool sync_all){
    settings.log_fd = log_fd;
    settings.log_to_stdout = log_to_stdout;
    settings.level = level;
    settings.sync_all = sync_all;
    settings.initialized = true;
}

void log_message(log_level level, const char *file, int line, const char *fmt, ...){
    if(!settings.initialized) return;
    if(level < settings.level) return;
    
    // TODO: add time support

    char* header_format_str = "[%s][%d][%s:%d] ";

    pthread_mutex_lock(&settings.lock);

    if(settings.log_to_stdout) printf(header_format_str,LOG_LEVEL_STR(level),(int) syscall( __NR_gettid ),file,line);
    if(settings.log_fd!=-1){
        // TODO: add error handling
        dprintf(settings.log_fd,header_format_str,LOG_LEVEL_STR(level),(int) syscall( __NR_gettid ),file,line);
    }
    
    va_list args1,args2;
    va_start(args1, fmt);
    va_start(args2, fmt);

    if(settings.log_to_stdout) vprintf(fmt,args1);
    if(settings.log_fd!=-1){
        // TODO: add error handling
        vdprintf(settings.log_fd,fmt,args2);
    }

    if(settings.log_to_stdout) printf("\n");
    if(settings.log_fd!=-1){
        dprintf(settings.log_fd,"\n");
    }

    if(settings.sync_all){
        fsync(settings.log_fd);
        fflush(stdout);
    }

    pthread_mutex_unlock(&settings.lock);

    va_end(args1);
    va_end(args2);
}
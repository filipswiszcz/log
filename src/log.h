#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

typedef struct {
    int type;
    const char* frmt;
    struct tm *time;
} log_event;

enum { DEBUG, INFO, WARN, FATAL };

#define log_debug(...) send(DEBUG, __VA_ARGS__)
#define log_info(...) send(INFO, __VA_ARGS__)
#define log_warn(...) send(WARN, __VA_ARGS__)
#define log_fatal(...) send(FATAL, __VA_ARGS__)

void send(int type, const char *frmt, ...);

#endif
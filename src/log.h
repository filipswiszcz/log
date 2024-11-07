#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define LOG_FILE_PATH "/var/log/run.log"

typedef struct {
    int type;
    const char* frmt;
    struct tm *time;
} log_event;

typedef struct log_queue_node log_queue_node;

struct log_queue_node {
    log_event* evt;
    log_queue_node* next;
};

typedef struct {
    log_queue_node* front;
    log_queue_node* rear;
    int size;
} log_queue;

log_queue log_queue_init();

bool is_log_queue_empty(log_queue* q);

int log_queue_size(log_queue* q);

log_event* log_queue_peek(log_queue* q);

void log_enqueue(log_queue* q, log_event evt);

log_event* log_dequeue(log_queue* q);

enum { DEBUG, INFO, WARN, FATAL };

#define log_debug(...) call_log_event(DEBUG, __VA_ARGS__)
#define log_info(...) call_log_event(INFO, __VA_ARGS__)
#define log_warn(...) call_log_event(WARN, __VA_ARGS__)
#define log_fatal(...) call_log_event(FATAL, __VA_ARGS__)

void call_log_event(int type, const char *frmt, ...);

#endif
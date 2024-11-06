#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int type;
    const char* frmt;
    struct tm *time;
} log_event;

typedef struct queue_node queue_node;

struct queue_node {
    log_event* evt;
    queue_node* next;
};

typedef struct {
    queue_node* front;
    queue_node* rear;
    int size;
} queue;

queue queue_init();

bool is_queue_empty(queue* q);

int queue_size(queue* q);

log_event* queue_peek(queue* q);

void enqueue(queue* q, log_event evt);

log_event* dequeue(queue* q);

enum { DEBUG, INFO, WARN, FATAL };

#define log_debug(...) call_event(DEBUG, __VA_ARGS__)
#define log_info(...) call_event(INFO, __VA_ARGS__)
#define log_warn(...) call_event(WARN, __VA_ARGS__)
#define log_fatal(...) call_event(FATAL, __VA_ARGS__)

void call_event(int type, const char *frmt, ...);

#endif
#include "log.h"


log_queue log_queue_init() {
    log_queue q = {
        .front = NULL,
        .rear = NULL,
        .size = 0,
    };
    return q;
}

bool is_log_queue_empty(log_queue* q) {
    return q -> front == NULL;
}

int log_queue_size(log_queue* q) {
    return q -> size;
}

log_event* log_queue_peek(log_queue* q) {
    if (is_log_queue_empty(q)) return NULL;
    return q -> front -> evt;
}

void log_enqueue(log_queue* q, log_event evt) {
    log_queue_node* nnod = malloc(sizeof(log_queue_node));
    log_event* nevt = malloc(sizeof(log_event));
    *nevt = evt;
    nnod -> evt = nevt;
    nnod -> next = NULL;
    if (is_log_queue_empty(q)) {
        q -> front = nnod;
        q -> rear = nnod;
    } else {
        q -> rear -> next = nnod;
        q -> rear = nnod;
    }
    q -> size++;
}

log_event* log_dequeue(log_queue* q) { // TODO remember to free(evt) event after use
    if (is_log_queue_empty(q)) return NULL;
    log_queue_node* nnod = q -> front;
    log_event* evt = q -> front -> evt;
    q -> front = q -> front -> next;
    q -> size--;
    if (is_log_queue_empty(q)) q -> rear = NULL;
    free(nnod);
    return evt;
}

static const char *type_names[] = {
    "DEBUG", "INFO", "WARN", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *type_colors[] = {
    "\x1b[36m", "\x1b[0m", "\x1b[33m", "\x1b[31m"
};
#endif

static void print_log_content(log_event *evt) {
    char buf[20];
    buf[strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", evt -> time)] = '\0';
#ifdef LOG_USE_COLOR
    printf(
        "%s [%s] %s%s \x1b[0m\n",
        buf,
        type_names[evt -> type],
        type_colors[evt -> type],
        evt -> frmt
    );
#else
    printf(
        "%s [%s] %s\n",
        buf,
        type_names[evt -> type],
        evt -> frmt
    );
#endif
#ifdef LOG_IN_FILE
    FILE *file = fopen(LOG_FILE_PATH, "a");
    if (file == NULL) {
        printf("Log file not found!\n");
        return;
    }
    fprintf(
        file,
        "%s [%s] %s\n",
        buf,
        type_names[evt -> type],
        evt -> frmt
    );
#endif
}

void call_log_event(int type, const char *frmt, ...) {
    time_t t = time(NULL);
    log_event evt = {
        .type = type,
        .frmt = frmt,
        .time = localtime(&t),
    };

    va_list args;
    va_start(args, frmt);

    while (*frmt) {
        if (*frmt == '%') {
            frmt++;
            if (*frmt == 'd') {
                printf("%d", va_arg(args, int));
            }
        } else frmt++;
    }

    va_end(args);

    print_log_content(&evt);
}
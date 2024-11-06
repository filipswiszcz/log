#include "log.h"


#ifdef LOG_IN_FILE
// TODO init global queue?
#endif

queue queue_init() {
    queue q = {
        .front = NULL,
        .rear = NULL,
        .size = 0,
    };
    return q;
}

bool is_queue_empty(queue* q) {
    return q -> front == NULL;
}

int queue_size(queue* q) {
    return q -> size;
}

log_event* queue_peek(queue* q) {
    if (is_queue_empty(q)) return NULL;
    return q -> front -> evt;
}

void enqueue(queue* q, log_event evt) {
    queue_node* nnod = malloc(sizeof(queue_node));
    log_event* nevt = malloc(sizeof(log_event));
    *nevt = evt;
    nnod -> evt = nevt;
    nnod -> next = NULL;
    if (is_queue_empty(q)) {
        q -> front = nnod;
        q -> rear = nnod;
    } else {
        q -> rear -> next = nnod;
        q -> rear = nnod;
    }
    q -> size++;
}

log_event* dequeue(queue* q) { // TODO remember to free(evt) event after use
    if (is_queue_empty(q)) return NULL;
    queue_node* nnod = q -> front;
    log_event* evt = q -> front -> evt;
    q -> front = q -> front -> next;
    q -> size--;
    if (is_queue_empty(q)) q -> rear = NULL;
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

static void print_msg(log_event *evt) {
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
}

void call_event(int type, const char *frmt, ...) {
    time_t t = time(NULL);
    log_event evt = {
        .type = type,
        .frmt = frmt,
        .time = localtime(&t),
    };
    print_msg(&evt);
}
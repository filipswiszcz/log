#include "log.h"


str_builder_t *str_builder_init(void) {
    str_builder_t *b;
    b = calloc(1, sizeof(*b));
    b -> content = malloc(32);
    *b -> content = '\0';
    b -> alloc = 32;
    b -> len = 0;
    return b;
}

void str_builder_destroy(str_builder_t *b) {
    if (b == NULL) return;
    free(b -> content);
    free(b);
}

void str_builder_check_space(str_builder_t *b, size_t l) {
    if (b == NULL || l == 0) return;
    if (b -> alloc >= b -> len + l + 1) return;
    while (b -> alloc < b -> len + l + 1) {
        b -> alloc <<= 1;
        if (b -> alloc == 0) b -> alloc--;
    }
    b -> content = realloc(b -> content, b -> alloc);
}

void str_builder_add_char(str_builder_t *b, char c) {
    if (b == NULL) return;
    str_builder_check_space(b, 1);
    b -> content[b -> len] = c;
    b -> len++;
    b -> content[b -> len] = '\0';
}

void str_builder_add_str(str_builder_t *b, char *s, size_t l) {
    if (b == NULL || s == NULL || *s == '\0') return;
    if (l == 0) l = strlen(s);
    str_builder_check_space(b, l);
    memmove(b -> content + b -> len, s, l);
    b -> len += l;
    b -> content[b -> len] = '\0';
}

void str_builder_add_void(str_builder_t *b, void *p) {
    if (b == NULL) return;
    char s[11];
    snprintf(s, sizeof(s), "%p", p);
    str_builder_add_str(b, s, 0);
}

void str_builder_add_int(str_builder_t *b, int v) {
    if (b == NULL) return;
    char s[12];
    snprintf(s, sizeof(s), "%d", v);
    str_builder_add_str(b, s, 0);
}

void str_builder_add_unsigned_int(str_builder_t *b, int v) {
    if (b == NULL) return;
    char s[12];
    snprintf(s, sizeof(s), "%u", v);
    str_builder_add_str(b, s, 0);
}

void str_builder_add_octal(str_builder_t *b, int v) {
    if (b == NULL) return;
    char s[12];
    snprintf(s, sizeof(s), "%o", v);
    str_builder_add_str(b, s, 0);
}

void str_builder_add_hex(str_builder_t *b, int v, int up) {
    if (b == NULL) return;
    char s[12];
    if (!up) snprintf(s, sizeof(s), "%x", v);
    else snprintf(s, sizeof(s), "%X", v);
    str_builder_add_str(b, s, 0);
}

void str_builder_add_double(str_builder_t *b, double v) {
    if (b == NULL) return;
    int l = snprintf(NULL, 0, "%f", v);
    char s[l + 1];
    snprintf(s, sizeof(s), "%f", v);
    str_builder_add_str(b, s, 0);
}

void str_builder_add_double_sn(str_builder_t *b, double v, int up) {
    if (b == NULL) return;
    int l;
    if (!up) l = snprintf(NULL, 0, "%e", v);
    else l = snprintf(NULL, 0, "%E", v);
    char s[l + 1];
    if (!up) snprintf(s, sizeof(s), "%e", v);
    else snprintf(s, sizeof(s), "%E", v);
    str_builder_add_str(b, s, 0);
}

void str_builder_add_double_csn(str_builder_t *b, double v, int up) {
    if (b == NULL) return;
    int l;
    if (!up) l = snprintf(NULL, 0, "%g", v);
    else l = snprintf(NULL, 0, "%G", v);
    char s[l + 1];
    if (!up) snprintf(s, sizeof(s), "%g", v);
    else snprintf(s, sizeof(s), "%G", v);
    str_builder_add_str(b, s, 0);
}

char *str_builder_get(str_builder_t *b) {
    char *s;
    if (b == NULL) return NULL;
    s = malloc(b -> len + 1);
    memcpy(s, b -> content, b -> len + 1);
    return s;
}

log_queue log_queue_init() {
    log_queue q = {
        .front = NULL,
        .rear = NULL,
        .size = 0,
    };
    return q;
}

bool is_log_queue_empty(log_queue *q) {
    return q -> front == NULL;
}

int log_queue_size(log_queue* q) {
    return q -> size;
}

log_event* log_queue_peek(log_queue *q) {
    if (is_log_queue_empty(q)) return NULL;
    return q -> front -> evt;
}

void log_enqueue(log_queue *q, log_event evt) {
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

log_event *log_dequeue(log_queue *q) { // TODO remember to free(evt) event after use
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
#ifdef LOG_USE_FILE
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

void call_log_event(int type, char *frmt, ...) {
    char *cont = frmt;
#ifdef LOG_USE_BUILDER
    va_list args;
    char *p, *sval;
    int ival;
    double dval;
    str_builder_t *builder;
    builder = str_builder_init();
    va_start(args, frmt);
    for (p = frmt; *p; p++) {
        if (*p != '%') {
            str_builder_add_char(builder, *p); continue;
        }
        switch (*++p) {
            case 'd':
                str_builder_add_int(builder, va_arg(args, int));
                break;
            case 'i':
                str_builder_add_int(builder, va_arg(args, int));
                break;
            case 'o':
                str_builder_add_octal(builder, va_arg(args, int));
                break;
            case 'x':
                str_builder_add_hex(builder, va_arg(args, int), 0);
                break;
            case 'X':
                str_builder_add_hex(builder, va_arg(args, int), 1);
                break;
            case 'u':
                str_builder_add_unsigned_int(builder, va_arg(args, int));
                break;
            case 'c':
                str_builder_add_char(builder, va_arg(args, int));
                break;
            case 's':
                str_builder_add_str(builder, va_arg(args, char*), 0);
                // for (sval = va_arg(args, char*); *sval; sval++) {
                //     str_builder_add_char(builder, *sval);
                // }
                break;
            case 'f':
                str_builder_add_double(builder, va_arg(args, double));
                break;
            case 'e':
                str_builder_add_double_sn(builder, va_arg(args, double), 0);
                break;
            case 'E':
                str_builder_add_double_sn(builder, va_arg(args, double), 1);
                break;
            case 'g':
                str_builder_add_double_csn(builder, va_arg(args, double), 0);
                break;
            case 'G':
                str_builder_add_double_csn(builder, va_arg(args, double), 1);
                break;
            case 'p':
                str_builder_add_void(builder, va_arg(args, void*));
                break;
            default:
                str_builder_add_char(builder, *--p);
                break;
        }
    }
    va_end(args);
    cont = str_builder_get(builder);
    str_builder_destroy(builder);
#endif
    time_t t = time(NULL);
    log_event evt = {
        .type = type,
        .frmt = cont,
        .time = localtime(&t),
    };
    print_log_content(&evt);
}
#include "log.h"

static const char *type_names[] = {
    "DEBUG", "INFO", "WARN", "FATAL"
};

#ifndef LOG_USE_COLOR
static const char *type_colors[] = {
    "\x1b[36m", "\x1b[0m", "\x1b[33m", "\x1b[31m"
};
#endif

static void x(log_event *evt) {
    char buf[20];
    buf[strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", evt -> time)] = '\0';
#ifndef LOG_USE_COLOR
    printf(
        "%s [%s] %s %s \x1b[0m\n",
        buf,
        type_names[evt -> type],
        type_colors[evt -> type],
        evt -> frmt
    );
#else
    printf(
        "%s %s %s\n",
        buf,
        type_names[evt -> type],
        evt -> frmt
    );
#endif
}

void send(int type, const char *frmt, ...) {
    time_t t = time(NULL);
    log_event evt = {
        .type = type,
        .frmt = frmt,
        .time = localtime(&t),
    };

    // try
    // printf("log %d", type);
    x(&evt);
}
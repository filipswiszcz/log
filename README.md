# log
Logging library implemented in C99
## Usage
Drop [log.h](https://github.com/filipswiszcz/log/blob/master/src/log.h) and [log.c](https://github.com/filipswiszcz/log/blob/master/src/log.c) into a project, use and compile
```
log_debug(...);
log_info(...);
log_warn(...);
log_fatal(...);
```
This results with output like this:
```
2024/10/26 01:02:24 [DEBUG] you're quite the silver tongue devil, aren't you?
```
### LOG_IN_FILE
Compile with ```-DLOG_IN_FILE``` to write output to a file (path defined in [log.h](https://github.com/filipswiszcz/log/blob/master/src/log.h))
### LOG_USE_COLOR
Compile with ```-DLOG_USE_COLOR``` to use ANSI color escape codes when printing
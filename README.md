# log
Logging library implemented in C99
## Usage
Copy [log.h](https://github.com/filipswiszcz/log/blob/master/src/log.h) and [log.c](https://github.com/filipswiszcz/log/blob/master/src/log.c) into a project
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
### LOG_USE_BUILDER
Compile with ```-DLOG_USE_BUILDER``` to use format specifiers
```
log_info("you're quite the %s tongue devil, aren't you?", "silver");
```
Format specifiers:

| Character     | Argument type               |
| ------------- | --------------------------- |
| d, i          | int; decimal                |
| o             | int; unsigned octal         |
| x, X          | int; unsigned hexadecimal   |
| u             | int; unsigned decimal       |
| c             | int; single character       |
| s             | char *; string until \0     |
| f             | double; [-]m.dddddd         |
| e, E          | double; [-]m.dddddd[e/E]±xx |
| g, G          | double; chooses %f or %e    |
| p             | void *; pointer             |

### LOG_USE_FILE
Compile with ```-DLOG_USE_FILE``` to write output to a file (path defined in [log.h](https://github.com/filipswiszcz/log/blob/master/src/log.h))
### LOG_USE_COLOR
Compile with ```-DLOG_USE_COLOR``` to use ANSI color escape codes
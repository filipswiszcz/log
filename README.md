# log
Logging library implemented in C23
## Usage
```
log_debug(...);
log_info(...);
log_warn(...);
log_fatal(...);
```
This result with output like this:
```
2024/10/26 01:02:24 [DEBUG] you're quite the silver tongue devil, aren't you?
```
###LOG_USE_COLOR
Compile with ```-DLOG_USE_COLOR``` to use ANSI color escape codes when printing
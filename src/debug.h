#ifndef DEBUG_H
#define DEBUG_H

int debug_print(const char * fmt, ...);

void debug_init(void);

void debug_putc(char c);

#endif /* DEBUG_H */
#define app_dbg_msg(format, ...) debug_printf(format, ##__VA_ARGS__)

void debug_printf(char *format, ...);

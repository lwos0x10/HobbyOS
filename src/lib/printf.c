#include <drivers/screen.h>
#include <lib/printf.h>

char *itoa(int num) {
    static char buffer[64];
    int is_negative = 0;
    int i = 0;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer; 
    }

    if (num < 0) {
        is_negative = 1;
        num *= -1;
    }

    while (num != 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }
    
    buffer[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    return buffer;
}

void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    char *s = va_arg(args, char *);
                    screen_print(s);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    screen_putc(c);
                    break;
                }
                case 'd': {
                    char *s = itoa(va_arg(args, int));
                    screen_print(s);
                    break;
                }
                case '%': {
                    screen_putc('%');
                    break;
                }
                default:
                    screen_putc('%');
                    screen_putc(*fmt);
                    break;
            }
        }
        else {
            screen_putc(*fmt);
        }
        fmt++;
    }
    va_end(args);
}

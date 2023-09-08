#include "printf.h"
#include "uart.h"

#define MAX_PRINT_SIZE 256
void print_int(int num, int base, int width, int zero_pad)
{
    char temp_buffer[MAX_PRINT_SIZE];
    int temp_index = MAX_PRINT_SIZE - 1;
    int num_chars = 0;

    do
    {
        temp_buffer[temp_index] = "0123456789abcdef"[num % base];
        temp_index--;
        num /= base;
        num_chars++;
    } while (num != 0);

    if (zero_pad)
    {
        while (width > num_chars)
        {
            temp_buffer[temp_index] = '0';
            temp_index--;
            num_chars++;
        }
    }
    else
    {
        while (width > num_chars)
        {
            temp_buffer[temp_index] = ' ';
            temp_index--;
            num_chars++;
        }
    }

    for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
    {
        uart_sendc(temp_buffer[i]);
    }
}
void print_double(double num, int width, int precision, int zero_pad)
{
    if (num < 0)
    {
        uart_sendc('-');
        num = -num;
        width--;
    }

    // Print integer part
    int int_part = (int)num;
    print_int(int_part, 10, width, zero_pad);

    // Print decimal point
    uart_sendc('.');

    // Print decimal part with specified precision
    double decimal_part = num - int_part;
    if (precision == -1)
    {
        precision = 0;
        double multiplier = 1.0;
        while (precision < 6 && (decimal_part * multiplier - (int)(decimal_part * multiplier)) > 1e-6)
        {
            precision++;
            multiplier *= 10.0;
        }
        for (int i = 0; i < precision; i++)
        {
            decimal_part *= 10;
            int digit = (int)decimal_part;
            uart_sendc('0' + digit);
            decimal_part -= digit;
        }
    }
    else
    {
        for (int i = 0; i < precision; i++)
        {
            decimal_part *= 10;
            int digit = (int)decimal_part;
            uart_sendc('0' + digit);
            decimal_part -= digit;
        }
    }
}

void printf(char *string, ...)
{
    va_list args;
    va_start(args, string);
    int buffer_index = 0;
    char buffer[MAX_PRINT_SIZE];

    while (*string != '\0')
    {
        if (*string == '%')
        {
            string++;

            // Initialize format specifiers
            int zero_pad = 0; // false
            int width = 0;
            int precision = -1;

            // Process width and 0 flag
            if (*string == '0')
            {
                zero_pad = 1;
                string++;
            }

            // Process width
            if (*string >= '0' && *string <= '9')
            {
                while (*string >= '0' && *string <= '9')
                {
                    width = width * 10 + (*string - '0');
                    string++;
                }
            }

            // Process precision
            if (*string == '.')
            {
                precision = 0;
                string++;
                while (*string >= '0' && *string <= '9')
                {
                    precision = precision * 10 + (*string - '0');
                    string++;
                }
            }

            // Process format specifiers
            switch (*string)
            {
            case 'd':
                print_int(va_arg(args, int), 10, width, zero_pad);
                break;
            case 'x':
                print_int(va_arg(args, int), 16, width, zero_pad);
                break;
            case 'o':
                print_int(va_arg(args, int), 8, width, zero_pad);
                break;
            case 'c':
                uart_sendc(va_arg(args, int));
                break;
            case 's':
            {
                char *str = va_arg(args, char *);
                uart_puts(str);
                break;
            }
            case 'f':
            {
                double num = va_arg(args, double);
                print_double(num, width, precision, zero_pad);
                break;
            }
            case '%':
                uart_sendc('%');
                break;
            }
            string++;
        }
        else
        {
            uart_sendc(*string);
            string++;
        }

        if (buffer_index == MAX_PRINT_SIZE - 1)
        {
            break;
        }
    }

    va_end(args);
}

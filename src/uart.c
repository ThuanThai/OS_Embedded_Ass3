#include "uart.h"

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init()
{
    unsigned int r;

    /* Turn off UART0 */
    UART0_CR = 0x0;

    /* Setup GPIO pins 14 and 15 */

    /* Set GPIO14 and GPIO15 to be pl011 TX/RX which is ALT0	*/
    r = GPFSEL1;
    r &= ~((7 << 12) | (7 << 15));      // clear bits 17-12 (FSEL15, FSEL14)
    r |= (0b100 << 12) | (0b100 << 15); // Set value 0b100 (select ALT0: TXD0/RXD0)
    GPFSEL1 = r;

    /* enable GPIO 14, 15 */
#ifdef RPI3    // RBP3
    GPPUD = 0; // No pull up/down control
    // Toogle clock to flush GPIO setup
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    }                                  // waiting 150 cycles
    GPPUDCLK0 = (1 << 14) | (1 << 15); // enable clock for GPIO 14, 15
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    }              // waiting 150 cycles
    GPPUDCLK0 = 0; // flush GPIO setup

#else // RPI4
    r = GPIO_PUP_PDN_CNTRL_REG0;
    r &= ~((3 << 28) | (3 << 30)); // No resistor is selected for GPIO 14, 15
    GPIO_PUP_PDN_CNTRL_REG0 = r;
#endif

    /* Mask all interrupts. */
    UART0_IMSC = 0;

    /* Clear pending interrupts. */
    UART0_ICR = 0x7FF;

    /* Set integer & fractional part of Baud rate
    Divider = UART_CLOCK/(16 * Baud)
    Default UART_CLOCK = 48MHz (old firmware it was 3MHz);
    Integer part register UART0_IBRD  = integer part of Divider
    Fraction part register UART0_FBRD = (Fractional part * 64) + 0.5 */

    // 115200 baud
    UART0_IBRD = 26;
    UART0_FBRD = 3;

    /* Set up the Line Control Register */
    /* Enable FIFO */
    /* Set length to 8 bit */
    /* Defaults for other bit are No parity, 1 stop bit */
    UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_8BIT;

    /* Enable UART0, receive, and transmit */
    UART0_CR = 0x301; // enable Tx, Rx, FIFO
}

unsigned int uart_isReadByteReady()
{
    return (*AUX_MU_LSR & 0x01);
}

/**
 * Send a character
 */
void uart_sendc(char c)
{

    /* Check Flags Register */
    /* And wait until transmitter is not full */
    do
    {
        asm volatile("nop");
    } while (UART0_FR & UART0_FR_TXFF);

    /* Write our data byte out to the data register */
    UART0_DR = c;
}

/**
 * Receive a character
 */
char uart_getc()
{
    char c = 0;

    /* Check Flags Register */
    /* Wait until Receiver is not empty
     * (at least one byte data in receive fifo)*/
    do
    {
        asm volatile("nop");
    } while (UART0_FR & UART0_FR_RXFE);

    /* read it and return */
    c = (unsigned char)(UART0_DR);

    /* convert carriage return to newline */
    return (c == '\r' ? '\n' : c);
}

/**
 * Display a string
 */
void uart_puts(char *s)
{
    while (*s)
    {
        /* convert newline to carriage return + newline */
        if (*s == '\n')
            uart_sendc('\r');
        uart_sendc(*s++);
    }
}

void uart_hex(unsigned int d)
{
    unsigned int n;
    int c;

    uart_puts("0x");
    for (c = 28; c >= 0; c = c - 4)
    {
        // Get highest 4-bit nibble
        n = (d >> c) & 0xF;

        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n += (n > 9) ? (-10 + 'A') : '0';
        uart_sendc(n);
    }
}

void uart_dec(int num)
{
    char buffer[12]; // Assuming a 32-bit integer won't exceed 11 digits
    int buffer_index = 0;
    int is_negative = 0;

    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }

    if (num == 0)
    {
        buffer[buffer_index] = '0';
        buffer_index++;
    }
    else
    {
        while (num > 0)
        {
            int digit = num % 10;
            buffer[buffer_index] = '0' + digit;
            buffer_index++;
            num /= 10;
        }
    }

    if (is_negative)
    {
        buffer[buffer_index] = '-';
        buffer_index++;
    }

    // Reverse the string in the buffer
    for (int i = 0; i < buffer_index / 2; i++)
    {
        char temp = buffer[i];
        buffer[i] = buffer[buffer_index - i - 1];
        buffer[buffer_index - i - 1] = temp;
    }

    // Send the string over UART
    for (int i = 0; i < buffer_index; i++)
    {
        uart_sendc(buffer[i]);
    }
}

unsigned char getAnyKey()
{
    volatile unsigned char ch = 0; // Declare ch as volatile
    if (uart_isReadByteReady())
        ch = uart_getc();
    return ch;
}
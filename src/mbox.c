// -----------------------------------mbox.c -------------------------------------
#include "mbox.h"
#include "gpio.h"
#include "uart.h"

/* Mailbox Data Buffer (each element is 32-bit)*/
/*
 * The keyword attribute allows you to specify special attributes
 *
 * The aligned(N) attribute aligns the current data item on an address
 * which is a multiple of N, by inserting padding bytes before the data item
 *
 * __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
 *
 *
 * We must ensure that our our buffer is located at a 16 byte aligned address,
 * so only the high 28 bits contain the address
 * (last 4 bits is ZERO due to 16 byte alignment)
 *
 */

volatile unsigned int __attribute__((aligned(16))) mBuf[36];

/**
 * Read from the mailbox
 */
uint32_t mailbox_read(unsigned char channel)
{
    // Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do
    {
        // Make sure there is mail to receive
        do
        {
            asm volatile("nop");
        } while (*MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = *MBOX0_READ;
    } while ((res & 0xF) != channel);
    return res;
}

/**
 * Write to the mailbox
 */
void mailbox_send(uint32_t msg, unsigned char channel)
{
    // Sending message is buffer_addr & channel number
    //  Make sure you can send mail
    do
    {
        asm volatile("nop");
    } while (*MBOX1_STATUS & MBOX_FULL);
    // send the message
    *MBOX1_WRITE = msg;
}

int mbox_call(unsigned int buffer_addr, unsigned char channel)
{
    unsigned int *buffer_ptr = (unsigned int *)((unsigned long)buffer_addr);
    unsigned int addressValue = buffer_addr;
    unsigned int *recoveredPointer = (unsigned int *)addressValue;
    *recoveredPointer++;
    // Check Buffer Address
    // Prepare Data (address of Message Buffer)
    unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);
    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel))
    {
        /* is it a valid successful response (Response Code) ? */
        if (*recoveredPointer == MBOX_RESPONSE)
        {
            return (1);
        }
    }
    return 0;
}

void mbox_buffer_setup(unsigned int buffer_addr, unsigned int tag_identifier,
                       unsigned int **res_data, unsigned int res_length,
                       unsigned int req_length, ...)
{
    // Set up the mailbox buffer with the provided parameters
    unsigned int *mbox = (unsigned int *)buffer_addr;

    // Initialize the mailbox buffer with zeros
    for (unsigned int i = 0; i < (req_length + res_length + 8) / 4; i++)
    {
        mbox[i] = 0;
    }

    // Populate the request values
    mbox[0] = 8 * 4;          // Total buffer size
    mbox[1] = MBOX_REQUEST;   // Request/response code
    mbox[2] = tag_identifier; // TAG identifier
    mbox[3] = req_length;     // Value buffer size
    mbox[4] = res_length;     // Response buffer size
    mbox[5] = 0;
    mbox[6] = 0;
    mbox[7] = MBOX_TAG_LAST;

    // Populate the list of request values using variable arguments
    va_list args;
    va_start(args, req_length);
    for (unsigned int i = 0; i < req_length / 4; i++)
    {
        mbox[5 + i] = va_arg(args, unsigned int);
    }
    va_end(args);

    // Set the response data pointer
    *res_data = mbox + 5 + req_length / 4;
}
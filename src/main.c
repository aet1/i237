#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"


#define BLINK_DELAY_MS 100

void main (void)
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init error console as stderr in UART3 and print user code info */
    uart3_init();
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    fprintf(stderr, "Version: %s built on: %s %s\n",
            GIT_DESCR, __DATE__, __TIME__);
    fprintf(stderr, "avr-libc version: %s\n", __AVR_LIBC_VERSION_STRING__);
    /* End UART3 init and info print */
    fprintf(stdout, STUD_NAME "\n");
    /* ASCII table print */
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};

    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);

    while (1) {
        /* Set pin 3 high to turn LED on */
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        /* Month name comparison and print */
        char inBuf = ' ';
        fprintf(stdout, "Enter Month name first letter >");
        fscanf(stdin, "%c", &inBuf);
        fprintf(stdout, "%c\n", inBuf);
        /* Set pin 3 low to turn LED off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}

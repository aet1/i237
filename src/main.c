#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include "hmi_msg.h"
#include "uart.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"


#define BLINK_DELAY_MS 100

void main (void)
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init error console as stderr in UART3 and print user code info */
    uart0_initialize();
    uart3_initialize();
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
    fprintf_P(stderr, PSTR(PRG_VER "\n"),
              GIT_DESCR, __DATE__, __TIME__);
    fprintf_P(stderr, PSTR(LIBC_VER "\n"), __AVR_LIBC_VERSION_STRING__,
              __VERSION__);
    /* End UART3 init and info print */
    fprintf_P(stdout, PSTR(STUD_NAME "\n"));
    lcd_puts_P(PSTR(STUD_NAME));
    /* ASCII table print */
    print_ascii_tbl(stdout);
    unsigned char ascii_table[128] = {0};

    for (unsigned char i = 0; i < sizeof(ascii_table); i++) {
        ascii_table[i] = i;
    }

    print_for_human(stdout, ascii_table, 128);

    while (1) {
        /* Set pin 3 high to turn LED on */
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        /* Month name comparison and print */
        char letter;
        fprintf_P(stdout, PSTR(GET_MONTH_NAME));
        fscanf(stdin, "%c", &letter);
        fprintf(stdout, "%c\n", letter);
        lcd_goto(0x40);

        for (int i = 0; i < 6; i++) {
            if (!strncmp_P(&letter, (PGM_P)pgm_read_word(&months[i]), 1)) {
                fprintf_P(stdout, (PGM_P)pgm_read_word(&months[i]));
                fputc('\n', stdout);
                lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
                lcd_putc(' ');
            }
        }

        lcd_puts_P(PSTR("                "));
        /* Set pin 3 low to turn LED off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}

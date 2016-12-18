#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include "hmi_msg.h"
#include "uart_wrapper.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "../lib/andygock_avr-uart/uart.h"

#define BLINK_DELAY_MS 100
#define BAUD 9600
#define COUNT

volatile uint32_t count_1;

static inline void init_uart(void) {
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init error console as stderr in UART3 and print user code info */
    uart0_init(BAUD_SELECT(BAUD, F_CPU));
    uart3_init(BAUD_SELECT(BAUD, F_CPU));
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();

}

static inline void init_count(void) {
    count_1 = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12);
    TCCR1B |= _BV(CS12);
    OCR1A = 62549;
    TIMSK1 |= _BV(OCIE1A);
}

static inline void start_print(void) {
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
    fprintf_P(stdout, PSTR(GET_MONTH_NAME));
}

static inline void print_month(void) {
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

static inline void heartbeat(void) {
    static uint32_t prev_time;
    uint32_t new_time;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        new_time = counter_1;
    }

    if (new_time != prev_time) {
        /* Toggle LED in Arduino Mega pin 25 */
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME_TEXT": %lu s \n"), new_time);
        prev_time = new_time;
    }
}


void main (void) {
    init_uart();
    init_count();
    sei();
    start_print();

    while (1) {
        heartbeat();
        if (uart0_available()) {
            print_month();
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    count_1++;
}

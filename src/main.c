/*  Copyright 2017 Aet Udusaar

    This file is part of i237.

    i237 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    i237 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with i237.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include "../lib/helius_microrl/microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "rfid.h"
#include "cli_microrl.h"

#define UART_BAUD 9600

// Create microrl object and pointer on it
static microrl_t rl;
static microrl_t *prl = &rl;

volatile uint32_t count_1;

static inline void init_uart(void)
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init error console as stderr in UART3 and print user code info */
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart3_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    stderr = &uart3_out;
    stdout = stdin = &uart0_io;
    lcd_init();
    lcd_clrscr();
}


static inline void init_cli(void)
{
    // Call init with ptr to microrl instance and print callback
    microrl_init (prl, cli_print);
    // Set callback for execute
    microrl_set_execute_callback (prl, cli_execute);
}


static inline void init_rfid_reader(void)
{
    MFRC522_init();
    PCD_Init();
}


static inline void init_count(void)
{
    count_1 = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12);
    TCCR1B |= _BV(CS12);
    OCR1A = 62549;
    TIMSK1 |= _BV(OCIE1A);
}


static inline void start_print(void)
{
    /*Print version libc info*/
    fprintf_P(stderr, PSTR(VER_FW "\n"));
    fprintf_P(stderr, PSTR(VER_LIBC " " VER_GCC"\n"));
    /* Print student name */
    fprintf_P(stdout, PSTR(STUD_NAME "\n"));
    lcd_puts_P(PSTR(STUD_NAME));
}


static inline void heartbeat(void)
{
    static uint32_t prev_time;
    uint32_t new_time;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        new_time = count_1;
    }

    if (new_time != prev_time) {
        /* Toggle LED in Arduino Mega pin 25 */
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME_TEXT": %lu s \n"), new_time);
        prev_time = new_time;
    }
}


void main (void)
{
    init_uart();
    init_count();
    sei();
    start_print();
    init_cli();
    init_rfid_reader();

    while (1) {
        heartbeat();
        microrl_insert_char (prl, cli_get_char());
    }
}


ISR(TIMER1_COMPA_vect)
{
    count_1++;
}

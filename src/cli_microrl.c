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
#include <string.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/andy_brown_memdebug/memdebug.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"
#include "rfid.h"

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))
#define UART_STATUS_MASK    0x00FF

void cli_print_help(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_rfid_read(const char *const *argv);
void cli_add(const char *const *argv);
void cli_remove(const char *const *argv);
void cli_display(const char *const *argv);
void cli_mem_stat(const char *const *argv);


typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1},
    {read_cmd, read_help, cli_rfid_read, 0},
    {add_cmd, add_help, cli_add, 1},
    {remove_cmd, remove_help, cli_remove, 1},
    {display_cmd, display_help, cli_display, 0},
    {mem_cmd, mem_help, cli_mem_stat, 0}
};


void cli_print(const char *str)
{
    printf("%s", str);
}


char cli_get_char(void)
{
    if (uart0_peek() != UART_NO_DATA) {
        return uart0_getc() & UART_STATUS_MASK;
    } else {
        return 0x00;
    }
}


void cli_print_help(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_HELP_MSG"\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        printf_P(cli_cmds[i].cmd);
        printf_P(PSTR(" : "));
        printf_P(cli_cmds[i].help);
        printf_P(PSTR("\n"));
    }
}


void cli_print_ver(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    printf_P(PSTR(VER_FW "\n"));
    printf_P(PSTR(VER_LIBC " " VER_GCC "\n"));
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    /* ASCII table print */
    print_ascii_tbl(stdout);
    unsigned char ascii_table[128] = {0};

    for (unsigned char i = 0; i < sizeof(ascii_table); i++) {
        ascii_table[i] = i;
    }

    print_for_human(stdout, ascii_table, 128);
}


void cli_handle_month(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    lcd_clr(0x40, 16);
    lcd_goto(0x40);

    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(argv[1], (PGM_P)pgm_read_word(&months[i]), strlen(argv[1]))) {
            fprintf_P(stdout, (PGM_P)pgm_read_word(&months[i]));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
            lcd_putc(' ');
        }
    }
}


void cli_rfid_read(const char *const *argv)
{
    (void) argv;
    Uid uid;
    Uid *uid_ptr = &uid;
    printf_P(PSTR("\n"));

    if (PICC_IsNewCardPresent()) {
        printf_P(PSTR(CARD_SELECTED_MSG "\n"));
        PICC_ReadCardSerial(uid_ptr);
        printf_P(PSTR(UID_SIZE_MSG "\n"), uid.size);
        printf_P(PSTR(UID_SAK_MSG"\n"), uid.sak);
        printf_P(PSTR(CARD_UID_MSG));

        for (byte i = 0; i < uid.size; i++) {
            printf("%02X", uid.uidByte[i]);
        }

        printf_P(PSTR("\n"));
    } else {
        printf_P((PSTR(CARD_NOT_SELECTED"\n")));
    }
}


void cli_add(const char *const *argv)
{
    (void) argv;
    rfid_add_card(argv[1]);
}


void cli_remove(const char *const *argv)
{
    (void) argv;
    rfid_remove_card(argv[1]);
}


void cli_display(const char *const *argv)
{
    (void) argv;
    rfid_display_cards();
}


void cli_mem_stat(const char *const *argv)
{
    (void) argv;
    extern int __heap_start, *__brkval;
    int v;
    int space;
    static int prev_space;
    space = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    printf_P(PSTR("Heap statistics\n"));
    printf_P(PSTR("Used: %d\n"), getMemoryUsed());
    printf_P(PSTR("Free: %d\n"), getFreeMemory());
    printf_P(PSTR("\nSpace between stack and heap:\n"));
    printf_P(PSTR("Current %d\n"), space);
    printf_P(PSTR("Previous %d\n"), prev_space);
    printf_P(PSTR("Change %d\n"), space - prev_space);
    printf_P(PSTR("\nFreelist\n"));
    printf_P(PSTR("Freelist size: %d\n"), getFreeListSize());
    printf_P(PSTR("Blocks in freelist: %d\n"), getNumberOfBlocksInFreeList());
    printf_P(PSTR("Largest block in freelist: %d\n"), getLargestBlockInFreeList());
    printf_P(PSTR("Largest freelist block: %d\n"),
             getLargestAvailableMemoryBlock());
    printf_P(PSTR("Largest allocable block: %d\n"), getLargestNonFreeListBlock());
    prev_space = space;
}


void cli_print_cmd_error(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(PRT_CMD_ERR"\n"));
}


void cli_print_cmd_arg_error(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(PRT_ARG_ERR"\n"));
}


int cli_execute(int argc, const char *const *argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}

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

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "hmi_msg.h"
#include "rfid.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "../lib/hd44780_111/hd44780.h"

typedef struct card {
    uint8_t uid_size;
    uint8_t uid[10];
    char *user;
    struct card *next;
} card_t;

card_t *head = NULL;


void rfid_add_card(const char *user)
{
    printf_P(PSTR("\n"));
    Uid uid;
    card_t *new_card;
    new_card = malloc(sizeof(card_t));

    if (new_card == NULL) {
        printf_P(PSTR(MEM_FAIL_MSG"\n"));
        free(new_card);
        return;
    }

    if (PICC_IsNewCardPresent()) {
        PICC_ReadCardSerial(&uid);
        new_card->user = malloc(strlen(user) + 1);

        if (new_card->user == NULL) {
            printf_P(PSTR(MEM_FAIL_MSG"\n"));
            free(new_card);
            return;
        }

        strcpy (new_card->user, user);
        new_card->uid_size = uid.size;
        memcpy(new_card->uid, &uid.uidByte, uid.size);

        if (head == NULL) {
            new_card->next = NULL;
        } else {
            new_card->next = head;
        }

        if (head != NULL) {
            card_t *current = head;

            while (current != NULL) {
                if (!memcmp(current->uid, new_card->uid, current->uid_size) ||
                        (!strcmp(current->user, new_card->user))) {
                    printf_P((PSTR(CARD_PRESENT_MSG "\n")));
                    return;
                }

                current = current->next;
            }
        }

        head = new_card;
        printf_P((PSTR(CARD_ADDED_MSG "\n")));
    } else {
        printf_P((PSTR(CARD_NOT_SELECTED"\n")));
    }
}


void rfid_print_card(const card_t *card)
{
    for (uint8_t i = 0; i < card->uid_size; i++) {
        printf("%02X", card->uid[i]);
    }

    printf(": %s", card->user);
}


void rfid_display_cards(void)
{
    printf_P(PSTR("\n"));

    if (head == NULL) {
        printf_P(PSTR(NO_CARDS_MSG "\n"));
    } else {
        card_t *current = head;

        while (current != NULL) {
            rfid_print_card(current);
            printf_P(PSTR("\n"));
            current = current->next;
        }
    }
}


void rfid_remove_card(const char *username)
{
    printf_P(PSTR("\n"));

    if (head == NULL) {
        printf_P(PSTR(NO_CARDS_MSG "\n"));
    }

    card_t *current = head;
    card_t *prev = NULL;

    while (current != NULL) {
        if (!strcmp(username, current->user)) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current);
            printf_P(PSTR(CARD_REMOVED_MSG "\n"));
            return;
        }

        prev = current;
        current = current->next;
    }

    printf_P(PSTR("\n"));
}


void doorlock(void)
{
    Uid uid;
    card_t *new_card;
    int ol = 0;

    if (PICC_IsNewCardPresent()) {
        PICC_ReadCardSerial(&uid);
        new_card = malloc(sizeof(card_t));

        if (new_card == NULL) {
            free(new_card);
            return;
        }

        new_card->user = NULL;
        new_card->uid_size = uid.size;
        memcpy(new_card->uid, &uid.uidByte, uid.size);
        new_card->next = NULL;

        if (head != NULL) {
            card_t *current = head;

            while (current != NULL) {
                if (!memcmp(current->uid, new_card->uid, current->uid_size)) {
                    lcd_clr(0x40, 16);
                    lcd_goto(0x40);
                    lcd_puts(current->user);
                    ol = 1;
                    return;
                }

                current = current->next;
            }
        }

        if (ol == 0) {
            lcd_clr(0x40, 16);
            lcd_goto(0x40);
            lcd_puts_P(PSTR(ACCESS_DENIED_MSG));
        }

        free (new_card);
    }
}

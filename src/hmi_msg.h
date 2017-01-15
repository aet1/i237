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

#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define STUD_NAME "Aet Udusaar"
#define GET_MONTH_NAME "Enter Month name first letter >"
#define VER_FW "Version: "GIT_DESCR " built on: " __DATE__" "__TIME__
#define VER_LIBC "avr-libc version: " __AVR_LIBC_VERSION_STRING__
#define VER_GCC "avr-gcc version: " __VERSION__
#define UPTIME_TEXT "Uptime is"
#define CLI_HELP_MSG "Implemented commands:"
#define PRT_CMD_ERR "Command not implemented.\n Use <help> to get help."
#define PRT_ARG_ERR "To few or too many arguments for this command\nUse <help>"
#define HELP_CMD "help"
#define HELP_HELP "Get help"
#define VER_CMD "version"
#define VER_HELP "Print FW version"
#define ASCII_CMD "ascii"
#define ASCII_HELP "print ASCII tables"
#define MONTH_CMD "month"
#define MONTH_HELP "Find matching month from lookup list. Usage: month <string>"
#define READ_CMD "read"
#define READ_HELP "Read card and print card data"
#define ADD_CMD "add"
#define ADD_HELP "Add a new card with username. Usage: add <string>"
#define REMOVE_CMD "remove"
#define REMOVE_HELP "Remove card from list. Usage: remove <string>"
#define DISPLAY_CMD "print"
#define DISPLAY_HELP "Print all cards"
#define MEM_CMD "memory"
#define MEM_HELP "Show memory usage"
#define CARD_SELECTED_MSG "Card selected"
#define UID_SIZE_MSG "UID size: 0x%02X"
#define UID_SAK_MSG "UID sak: 0x%02X"
#define CARD_UID_MSG "Card UID: "
#define CARD_NOT_SELECTED "Unable to select card"
#define MEM_FAIL_MSG "Memory operation failed"
#define CARD_PRESENT_MSG "Card is already in the list"
#define CARD_ADDED_MSG "Card has been added to the list"
#define NO_CARDS_MSG "No cards in the list"
#define CARD_REMOVED_MSG "Card was successfully removed"
#define ACCESS_DENIED_MSG "Access denied"

extern PGM_P const months[];

extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];
extern const char read_cmd[];
extern const char read_help[];
extern const char add_cmd[];
extern const char add_help[];
extern const char remove_cmd[];
extern const char remove_help[];
extern const char display_cmd[];
extern const char display_help[];
extern const char mem_cmd[];
extern const char mem_help[];

#endif /*_HMI_MSG_H_*/

#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define STUD_NAME "Aet Udusaar"
#define GET_MONTH_NAME "Enter Month name first letter >"
#define VER_FW "Version: "GIT_DESCR " built on: " __DATE__" "__TIME__
#define VER_LIBC "\n avr-libc version: " __AVR_LIBC_VERSION_STRING__
#define VER_GCC "\n avr-gcc version: " __VERSION__
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

extern PGM_P const months[];

extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];

#endif /*_HMI_MSG_H_*/

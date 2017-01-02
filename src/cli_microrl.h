#ifndef _CLI_MICRORL_H_
#define _CLI_MICRORL_H_

int cli_execute(int argc, const char *const *argv);
void cli_print(const char * str);
char cli_get_char(void);
void cli_print_help(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_month(const char *const *argv);

#endif /* _CLI_MICRORL_H_ */

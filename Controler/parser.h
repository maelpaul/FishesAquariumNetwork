#ifndef _PARSER_H_
#define _PARSER_H_


struct command {
    char* command_name;
    int nb_params;
    char** params;
    int _total_param_len;
};

void parse_command(struct command * command, char** input, int argc);

void free_command(struct command * command);

void print_command(struct command * command);

#endif
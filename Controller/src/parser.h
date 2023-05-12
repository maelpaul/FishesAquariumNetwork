#ifndef _PARSER_H_
#define _PARSER_H_


struct command {
    char* command_name;
    int nb_params;
    char** params;
    int _total_param_len;
};


/* This function initializes the fields of a 'command struct'
*
* @param struct command * command : a pointer pointing to the sctruct to initialize
*/
void init_command(struct command * command);

/* This function parses the command and completes a 'command struct' which will be used later by the server
*
* @param struct command * command : a pointer pointing to the sctruct to complete
* @param char** imput : the input string (e.g add view 52 5x5+5+5)
* @param argc : the number of arguments given (is reduced by 1 because it includes the ./xxx name)
* @returns : 1 if the input is correct, 0 otherwise
*/
int parse_command(struct command * command, char** argv, int argc);


/* This function prints a 'command struct'
*
* @param struct command * command : a pointer pointing to the sctruct to print
*/
void print_command(struct command * command);

/* This function tests if the parameter given when using the command "add view" is given in the correct format
*
* @param char * input : a string which is the parameter used with the command "add view" (e.g. 5x5+5+5)
* @returns : 1 if the format is correct, 0 otherwise
*/
int check_add_wiew_format(char * input);

/* This function frees a 'command struct'
*
* @param struct command * command : a pointer pointing to the sctruct to free
*/
void free_command(struct command * command);

#endif
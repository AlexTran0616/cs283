#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (clist == NULL || cmd_line == NULL) {
        return EXIT_NOT_IMPL;
    }

    memset(clist, 0, sizeof(command_list_t));

    int cmd_count = 0;
    char *start = cmd_line;
    char *end;

    while ((end = strchr(start, PIPE_CHAR)) != NULL || (end = strchr(start, '\0')) != NULL)
    {
        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        char command[EXE_MAX + ARG_MAX];
        strncpy(command, start, end - start);
        command[end - start] = '\0';

        char *cmd_start = command;
        char *cmd_end = command + strlen(command) - 1;
        
        while (*cmd_start == SPACE_CHAR && cmd_start < cmd_end) {
            cmd_start++;
        }

        while (*cmd_end == SPACE_CHAR && cmd_end > cmd_start) {
            *cmd_end = '\0';
            cmd_end--;
        }

        char exe[EXE_MAX];
        char args[ARG_MAX];
        int i = 0, j = 0;
        
        while (cmd_start[i] != SPACE_CHAR && cmd_start[i] != '\0') {
            exe[i] = cmd_start[i];
            i++;
        }
        exe[i] = '\0';

        if (cmd_start[i] == SPACE_CHAR) {
            i++;
            while (cmd_start[i] != '\0') {
                args[j] = cmd_start[i];
                i++;
                j++;
            }
        }
        args[j] = '\0';

        strncpy(clist->commands[cmd_count].exe, exe, EXE_MAX - 1);
        strncpy(clist->commands[cmd_count].args, args, ARG_MAX - 1);

        clist->num++;
        cmd_count++;
        
        if (*end == '\0') {
            break;
        }
        start = end + 1;
    }

    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }

    return OK;
}


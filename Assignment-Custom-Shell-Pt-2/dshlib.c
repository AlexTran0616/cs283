#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    int argc = 0;
    int in_quotes = 0;

    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    cmd_buff->_cmd_buffer = strdup(cmd_line);

    char *p = cmd_buff->_cmd_buffer;
    char *arg_start = NULL;

    while (*p) {
        if (*p == '"') {
            in_quotes = !in_quotes;
            if (in_quotes) {
                arg_start = ++p;
            } else {
                *p = '\0';
                cmd_buff->argv[argc++] = arg_start;
                arg_start = NULL;
                p++;
            }
        } else if (!in_quotes && isspace((unsigned char)*p)) {
            if (arg_start) {
                *p = '\0';
                cmd_buff->argv[argc++] = arg_start;
                arg_start = NULL;
            }
            p++;
        } else {
            if (!arg_start) {
                arg_start = p;
            }
            p++;
        }

        if (argc >= CMD_ARGV_MAX) {
            fprintf(stderr, "error: too many arguments\n"); 
            return ERR_CMD_ARGS_BAD;
        }
    }

    if (arg_start) {
        cmd_buff->argv[argc++] = arg_start;
    }

    cmd_buff->argc = argc;
    cmd_buff->argv[argc] = NULL;

    if (in_quotes) {
        fprintf(stderr, "error: unmatched quotes\n");
        return -ERR_EXEC_CMD;
    }

    return OK;
}






Built_In_Cmds match_command(const char *input)
{
    if (strcmp(input, "exit") == 0) {
        return BI_CMD_EXIT;
    }
    if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    }
    if (strcmp(input, "rc") == 0) {
        return BI_RC;
    }
    return BI_NOT_BI;
}






Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds cmd_type = match_command(cmd->argv[0]);
    switch (cmd_type) {
        case BI_CMD_EXIT:
            exit(0);
            break;
        case BI_CMD_CD:
            if (cmd->argc == 2) {
                if (chdir(cmd->argv[1]) != 0) {
                    perror("cd");
                    return ERR_EXEC_CMD; 
                }
                return BI_CMD_CD;
            } else {
                fprintf(stderr, "cd: expected argument\n");
                return 1;  
            }
            break;
        default:
            break;
    }
    return cmd_type;
}







int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(ARG_MAX);
    if (cmd_buff == NULL) {
        return ERR_MEMORY;
    }
    cmd_buff_t cmd;

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        char *start = cmd_buff;
        while (isspace((unsigned char)*start)) start++;
        if (*start == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        char *end = start + strlen(start) - 1;
        while (end > start && isspace((unsigned char)*end)) end--;
        end[1] = '\0';

        build_cmd_buff(start, &cmd);

        if (cmd.argc > 0) {
            Built_In_Cmds cmd_type = match_command(cmd.argv[0]);
            if (cmd_type == BI_CMD_EXIT) {
                break;
            }
            if (cmd_type == BI_CMD_CD) {
                if (cmd.argc == 1) {
                    continue;
                } else if (cmd.argc == 2) {
                    if (chdir(cmd.argv[1]) != 0) {
                        perror("cd");
                    }
                    continue;
                }
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            execvp(cmd.argv[0], cmd.argv);
            fprintf(stderr,  "Command not found in PATH\n");
            exit(ERR_EXEC_CMD);
	} else if (pid > 0) {
           wait(NULL);

	} else {
            perror("fork");
        }
    }

    free(cmd_buff);
    return OK;
}


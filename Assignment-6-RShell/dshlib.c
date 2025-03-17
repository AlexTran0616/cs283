#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 *
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    int argc = 0;
    int in_quotes = 0;

    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    cmd_buff->_cmd_buffer = strdup(cmd_line);

    char *p = cmd_buff->_cmd_buffer;
    char *arg_start = NULL;

    while (*p) {
        if (*p == '"' || *p == '\'') {
            char quote_type = *p;
            in_quotes = !in_quotes;
            if (in_quotes) {
                arg_start = ++p;
            } else {
                if (*p == quote_type) {
                    *p = '\0';
                    cmd_buff->argv[argc++] = arg_start;
                    arg_start = NULL;
                }
                p++;
            }
        } else if (!in_quotes && (*p == '<' || *p == '>' || (*p == '>' && *(p + 1) == '>'))) {
            if (*p == '>' && *(p + 1) == '>') {
                cmd_buff->argv[argc++] = ">>";
                p += 2;
            } else {
                cmd_buff->argv[argc++] = (*p == '<') ? "<" : ">";
                p++;
            }
            while (isspace((unsigned char)*p)) p++;

            cmd_buff->argv[argc++] = p;
            while (*p && !isspace((unsigned char)*p)) p++;
            if (*p) *p++ = '\0';
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
        return ERR_EXEC_CMD;
    }

    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    }
    if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
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
                    fprintf(stderr, "cd: %s: No such file or directory\n", cmd->argv[1]);
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

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    clist->num = 0;
    char *cmd_start = strtok(cmd_line, PIPE_STRING);

    while (cmd_start != NULL) {
        if (clist->num >= CMD_MAX) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }
        build_cmd_buff(cmd_start, &clist->commands[clist->num]);
        clist->num++;
        cmd_start = strtok(NULL, PIPE_STRING);
    }

    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int num_cmds = clist->num;
    int pipe_fd[2];
    int in_fd = 0;

    for (int i = 0; i < num_cmds; i++) {
        pipe(pipe_fd);
        pid_t pid = fork();
        if (pid == 0) {
            int j = 0;
            while (clist->commands[i].argv[j] != NULL) {
                if (strcmp(clist->commands[i].argv[j], "<") == 0) {
                    int fd_in = open(clist->commands[i].argv[j + 1], O_RDONLY);
                    if (fd_in == -1) {
                        perror("open input file");
                        exit(ERR_EXEC_CMD);
                    }
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                    clist->commands[i].argv[j] = NULL;
                    clist->commands[i].argv[j + 1] = NULL;
                } else if (strcmp(clist->commands[i].argv[j], ">") == 0) {
                    int fd_out = open(clist->commands[i].argv[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd_out == -1) {
                        perror("open output file");
                        exit(ERR_EXEC_CMD);
                    }
                    dup2(fd_out, STDOUT_FILENO);
                    close(fd_out);
                    clist->commands[i].argv[j] = NULL;
                    clist->commands[i].argv[j + 1] = NULL;
                } else if (strcmp(clist->commands[i].argv[j], ">>") == 0) {
                    int fd_out = open(clist->commands[i].argv[j + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fd_out == -1) {
                        perror("open output file");
                        exit(ERR_EXEC_CMD);
                    }
                    dup2(fd_out, STDOUT_FILENO);
                    close(fd_out);
                    clist->commands[i].argv[j] = NULL;
                    clist->commands[i].argv[j + 1] = NULL;
                }
                j++;
            }

            dup2(in_fd, STDIN_FILENO);
            if (i < num_cmds - 1) {
                dup2(pipe_fd[1], STDOUT_FILENO);
            }
            close(pipe_fd[0]);
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(ERR_EXEC_CMD);
        } else if (pid > 0) {
            waitpid(pid, NULL, 0);
            close(pipe_fd[1]);
            in_fd = pipe_fd[0];
        } else {
            perror("fork");
            return ERR_EXEC_CMD;
        }
    }

    return OK;
}

int exec_local_cmd_loop() {
    char *cmd_buff = malloc(SH_CMD_MAX);
    if (cmd_buff == NULL) {
        return ERR_MEMORY;
    }

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
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

        cmd_buff_t cmd;
        if (build_cmd_buff(start, &cmd) == ERR_EXEC_CMD) {
            continue;
        }

        Built_In_Cmds cmd_type = exec_built_in_cmd(&cmd);
        if (cmd_type == BI_CMD_EXIT) {
            printf("exiting...\n");
            break;
        } else if (cmd_type != BI_NOT_BI) {
            continue;
        }

        command_list_t cmd_list;
        int result = build_cmd_list(start, &cmd_list);
        if (result != OK) {
            continue;
        }

        execute_pipeline(&cmd_list);
    }

    free(cmd_buff);
    return OK;
}

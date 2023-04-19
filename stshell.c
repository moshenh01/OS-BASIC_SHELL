#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

# define MAX_ARGS 10
pid_t child_pid = 0;
char *history[100];
int history_length = 0;
int current_index = 0;
int history_index = 0;
int history_flag = 0;



int execute_pipe_commamd(char **command, char *input_file, char *output_file, int append)
{

	if (input_file != NULL)
	{
		int fd = open(input_file, O_RDONLY);
		if (fd == -1)
		{
			perror("input file open failed");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	// Redirect output if necessary
	if (output_file != NULL)
	{
		int fd;
		if (append)
		{
			fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else
		{
			fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		if (fd == -1)
		{
			perror("output file open failed");
			return 1;
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}

	// Execute command

	if (execvp(command[0], command) == -1)
	{
		perror("execvp failed");
		return 1;
		exit(EXIT_FAILURE);
	}
	return 0;
}

void execute_command(char **command, char *input_file, char *output_file, int append, int pipefd[2])
{
	pid_t pid;
	int status;
	// printf("1)Executing command: %s %s %s %d %d  \n", command[0], command[1], command[2], pipefd[0], pipefd[1]);
	// printf("1)Executing command: %s %s %s %s %s %d %d  \n", command[0], command[1], command[2],command[3],command[4], pipefd[0], pipefd[1]);

	pid = fork();

	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		// Child process

		// use pipe if necessary
		//  if (pipefd[0] != -1 && pipefd[1] != -1)
		//  {

		// 	close(pipefd[0]);
		// 	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		// 	{
		// 		perror("dup2 failed");
		// 		exit(EXIT_FAILURE);
		// 	}
		// 	close(pipefd[1]);
		// }
		signal(SIGINT, SIG_DFL);
		// Redirect input if necessary
		if (input_file != NULL)
		{
			int fd = open(input_file, O_RDONLY);
			if (fd == -1)
			{
				perror("input file open failed");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		// Redirect output if necessary
		if (output_file != NULL)
		{
			int fd;
			if (append)
			{
				fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			}
			else
			{
				fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			}
			if (fd == -1)
			{
				perror("output file open failed");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		// Execute command

		if (execvp(command[0], command) == -1)
		{
			perror("execvp failed");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		printf("Error: could not fork\n");
		exit(1);
	}
	else
	{
		// Parent process

		// use pipe if necessary
		//  if (pipefd[0] != -1 && pipefd[1] != -1)
		//  {
		//  	close(pipefd[1]);
		//  	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		//  	{
		//  		perror("dup2 failed");
		//  		exit(EXIT_FAILURE);
		//  	}
		//  	close(pipefd[0]);
		//  }

		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid failed");
			exit(EXIT_FAILURE);
		}

		if (WIFEXITED(status))
		{
			printf("%s+ returned %d\n", command[0], WEXITSTATUS(status));
		}
		else
		{
			printf("%s+ exited abnormally\n", command[0]);
		}
	}
}

void sig_hendler(int signum)
{
	if (signum == SIGINT)
	{
		// printf("ignore ctrl+c \n");
		// ignore ctrl+c for the parent process
		// if (child_pid != 0)
		// {
		// 	kill(child_pid, SIGINT);
		// }
	}
	if (signum == SIGUSR1)
	{
		// do something
	}
}
void print_arry_arg(int arry_arg[], int num_commands)
{
	for (int i = 0; i < num_commands; i++)
	{
		printf("arry_arg[%d] = %d \n", i, arry_arg[i]);
	}
}

void print_commands(char *commands[10][10], int num_commands, int arry_arg[])
{
	for (int i = 0; i < num_commands; i++)
	{
		printf("commands[%d] = ", i);
		for (int j = 0; j < arry_arg[i]; j++)
		{
			printf("%s ", commands[i][j]);
		}
		printf("\n");
	}
}

void add_command_to_history(char *commands[10][10], int num_commands, int arry_arg[])
{
	if (commands[0][0] == NULL || commands[0] == NULL || strcmp(commands[0][0], "") == 0)
		return;

	char *command = (char *)calloc(1024, sizeof(char));
	for (int i = 0; i < num_commands; i++)
	{
		for (int j = 0; j < arry_arg[i]; j++)
		{

			strcat(command, commands[i][j]);
			strcat(command, " ");
		}
		if (i != num_commands - 1)
			strcat(command, "| ");
	}
	strcat(command, "\0");
	history[history_index] = (char *)malloc(strlen(command) + 1);
	// printf("command = %s \n", command);
	strcpy(history[history_index], command);
	history_index = (history_index + 1) % 100;

	if (history_index == 100)
		history_flag = 1;

	if (history_flag == 1)
		history_length = 100;
	else
		history_length = history_index;

	free(command);
}
void free_history()
{
	for (int i = 0; i < history_length; i++)
	{
		free(history[i]);
	}
}

void print_history()
{
	for (int i = 0; i < history_index-1; i++)
	{
		printf("%d: %s\n", i, history[i]);
	}
}

void print_output_file(char *input_files[], int num_commands)
{
	for (int i = 0; i < num_commands; i++)
	{
		printf("input_files[%d] = %s \n", i, input_files[i]);
	}
}

int main()
{

	char input_stream[1024];
	char *commands[10][10];
	char *token;
	int i = 0;
	// 0: overwrite, 1: append
	int num_commands = 0; // number of commands.
	int num_args = 0;	  // number of arguments for the command.

	int orig_stdin = dup(STDIN_FILENO);	  // save original stdin file descriptor
	int orig_stdout = dup(STDOUT_FILENO); // save original stdout file descriptor
	int arry_arg[10];					  // number of arguments for each command
	char *input_files[10];				  // input files for each command
	char *output_file[10];				  // output file for each command
	int append_flag[10];				  // append flag for each command

	// ignore ctrl+c
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}

	while (1)
	{
		printf("myshell> ");
		fgets(input_stream, 1024, stdin);
		input_stream[strlen(input_stream) - 1] = '\0'; // replace \n with \0

		/* parse command line */
		// Reset variables for new command line input.
	
		
		
		for (i = 0; i < 10; i++)
		{
			arry_arg[i] = 0;
			input_files[i] = NULL;
			output_file[i] = NULL;
			append_flag[i] = 0;
		}
		num_commands = 0;
		num_args = 0;
		token = strtok(input_stream, " ");
		while (token != NULL)
		{
			if (token[0] == '"')
			{
				// If the token starts with a quote, it is a string.
				// Concatenate the rest of the string until the next quote.
				char string[1024];
				bzero(string, 1024);
				strcat(string, token);
				token = strtok(NULL, " ");
				while (token != NULL && token[strlen(token) - 1] != '"')
				{
					strcat(string, " ");
					strcat(string, token);
					token = strtok(NULL, " ");
				}
				if (token != NULL)
				{
					strcat(string, " ");
					strcat(string, token);
				}
				// Remove the quotes from the string.
				string[strlen(string) - 1] = '\0';
				for(int i = 1; i < strlen(string); i++)
				{
					string[i-1] = string[i];
				}
				string[strlen(string) - 1] = '\0';
				
				token = string;
				
				

				

				
	
			}

			if (strcmp(token, ">") == 0)
			{
				// Output redirection, catch the next token as output file
				token = strtok(NULL, " ");
				output_file[num_commands - 1] = token;
				token = strtok(NULL, " ");
				append_flag[num_commands - 1] = 0;
			}
			else if (strcmp(token, ">>") == 0)
			{
				// Output redirection, catch the next token as output file
				token = strtok(NULL, " ");
				output_file[num_commands - 1] = token;
				token = strtok(NULL, " ");
				append_flag[num_commands - 1] = 1;
			}
			else if (strcmp(token, "<") == 0)
			{
				// Input redirection, catch the next token as input file
				token = strtok(NULL, " ");
				input_files[num_commands - 1] = token;
				token = strtok(NULL, " ");
			}
			else if (strcmp(token, "|") == 0)
			{
				// Pipe, from new its a new command
				arry_arg[num_commands - 1] = num_args;
				commands[num_commands - 1][num_args] = NULL; // NULL terminate the last command and argument

				num_args = 0;
				token = strtok(NULL, " ");
				continue;
			}
			else
			{
				// Command or argument
				if (num_args == 0)
				{
					// Command
					commands[num_commands][num_args++] = token;
					num_commands++;
				}
				else
				{
					// Argument
					commands[num_commands - 1][num_args++] = token;
				}
			}
			token = strtok(NULL, " ");
		}
		arry_arg[num_commands - 1] = num_args;

		commands[num_commands - 1][num_args] = NULL; // NULL terminate the last command and argument

		//print_arry_arg(arry_arg, num_commands);
		//print_commands(commands, num_commands, arry_arg);
		//print_output_file(output_file, num_commands);
		add_command_to_history(commands, num_commands, arry_arg);

		/* Is command empty or ..*/
		if (commands[0] == NULL || commands[0][0] == NULL || strcmp(commands[0][0], "") == 0 || strcmp(commands[0][0], "\n") == 0)
			continue;

		if (strcmp(commands[0][0], "exit") == 0)
		{
			free_history();
			exit(0);
		}

		if (num_commands == 1)
		{
			i = 0;

			if (strcmp(commands[0][0], "cmp") == 0)
			{

				if (arry_arg[i] > 5 || arry_arg[i] < 3)
				{
					printf("Usage: cmp file1 file2 -i/-v\n");
					continue;
				}
				commands[0][0] = "./cmp";
				execute_command(commands[0], input_files[0], output_file[0], append_flag[0], NULL);
			}
			// execute copy tool if user types "copy"
			else if (strcmp(commands[0][0], "copy") == 0)
			{
				if (arry_arg[i] > 5 || arry_arg[i] < 3)
				{
					printf("Usage: copy file1 file2 -v\n");
					continue;
				}
				commands[0][0] = "./copy";
				execute_command(commands[i], input_files[0], output_file[0], append_flag[0], NULL);
			}

			else if (strcmp(commands[0][0], "encode") == 0)
			{
				if (arry_arg[i] > 3 || arry_arg[i] < 2)
				{
					printf("Usage: encode <codec> <message>\n");
					continue;
				}

				commands[i][0] = "./encode";
				execute_command(commands[0], input_files[0], output_file[0], append_flag[0], NULL);
			}
			else if (strcmp(commands[i][0], "decode") == 0)
			{
				if (arry_arg[i] > 3 || arry_arg[i] < 2)
				{
					printf("Usage: decode <codec> <message>\n");
					continue;
				}

				commands[i][0] = "./decode";
				execute_command(commands[i], input_files[0], output_file[0], append_flag[0], NULL);
			}
			//history command
			else if (strcmp(commands[0][0], "history") == 0)
			{
				
				if (arry_arg[i] == 1)
				{
					print_history();
				}
			
			}
			else
			{

				// Execute command
				// printf("Executing command: %s\n", commands[i][0]);
				execute_command(commands[i], input_files[0], output_file[0], append_flag[0], NULL);
			}
		}

		else if (num_commands == 2)
		{
			pid_t pid1, pid2;
			int status1, status2;
			int fd[2];

			pid1 = fork();
			if (pipe(fd) < 0)
			{
				printf("Error creating pipe\n");
				exit(1);
			}
			if (pid1 == 0)
			{
				// Child

				pid2 = fork();
				if (pid2 == 0)
				{
					signal(SIGINT, SIG_DFL);
					// grand child

					close(fd[0]);
					dup2(fd[1], STDOUT_FILENO);
					close(fd[1]);
					execute_pipe_commamd(commands[0], input_files[0], output_file[0], append_flag[0]);
					exit(0);
				}

				else if (pid2 < 0)
				{
					printf("Error in fork()\n");
					exit(1);
				}
				else
				{

					// child
					close(fd[1]);
					dup2(fd[0], STDIN_FILENO);
					close(fd[0]);
					if (execute_pipe_commamd(commands[1], input_files[1], output_file[1], append_flag[1]) < 0)
					{
						printf("Error executing command 2\n");
						exit(1);
					}
				}
			}
			else if (pid1 < 0)
			{
				printf("Error in fork()\n");
				exit(1);
			}
			else
			{
				close(fd[0]);
				close(fd[1]);
				// Parent shell
				waitpid(pid1, &status1, 0);
				waitpid(pid2, &status2, 0);
			}
		}

		//just like you asked.
		else if(num_commands == 3){
			int fd1[2]; 
			int fd2[2];	
			int pip_status;
			int status;
			pid_t pid1, pid2;
			
			pip_status = pipe(fd1);//pipe 2
			if(pip_status < 0){
				printf("Error creating pipe\n");
				exit(1);
			}
			pid1 = fork();
			if ( pid1 < 0 ) 
			{
				puts("Fork Error : 1");
				exit(1);
			}
			else if (pid1 == 0)
			{
				//child
				signal(SIGINT, SIG_DFL);
				dup2(fd1[1], STDOUT_FILENO);// redirect stdout to pipe write end.
				close(fd1[0]);
				if(pipe(fd2) < 0){//pipe 1
					printf("Error creating pipe\n");
					exit(1);
				}
				pid2 = fork();
				if ( pid2 < 0 ) 
				{
					puts("Fork Error : 2");
					exit(1);
				}
				else if (pid2 == 0)
				{
					//grand child
					dup2(fd2[1], STDOUT_FILENO);// redirect stdout to pipe write end.
					close(fd2[0]);
					if(execute_pipe_commamd(commands[0], input_files[0], output_file[0], append_flag[0]) < 0){
						printf("Error executing command 1\n");
						exit(1);
					}
					exit(0);
				}
				else
				{
					//child
					dup2(fd2[0], STDIN_FILENO);// redirect stdin to pipe read end.
					close(fd2[1]);
					if(execute_pipe_commamd(commands[1], input_files[1], output_file[1], append_flag[1]) < 0){
						printf("Error executing command 2\n");
						exit(1);
					}
					exit(0);
				}

			}
			else
			{
				//parent
				dup2(fd1[0], STDIN_FILENO);// redirect stdin to pipe read end.
				close(fd1[1]);
				if(execute_pipe_commamd(commands[2], input_files[2], output_file[2], append_flag[2]) < 0){
					printf("Error executing command 3\n");
					exit(1);
				}
				wait(&status);
			}
			
			

			
		}
		//this opt is for me not a part of the assignment
		else if(num_commands > 3)
		{
			int num_pipe = num_commands - 1;
			int pipes[num_pipe][2];
			
            for (int i = 0; i < num_pipe; i++)
            {
                if (pipe(pipes[i]) == -1)
                {
                    perror("pipe");
                    exit(1);
                }
            }

			int pid;
            for (int i = 0; i <  num_commands; i++)
            {
                pid = fork();

                if (pid == -1)
                {
                    perror("fork");
                    exit(1);
                }
                else if (pid == 0)
                {
                    // Child process

                    // Set up signal handling for Ctrl+c
                    signal(SIGINT, SIG_DFL);

                    // Redirect input from the previous pipe, if this is not the first command
                    if (i > 0)
                    {
                        if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) // duplciate the read end of the previous pipe to the standard input
                        {
                            perror("dup2");
                            exit(1);
                        }
                    }

                    // Redirect output to the next pipe, if this is not the last command
                    if (i < num_pipe)
                    {
                        if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
                        {
                            perror("dup2");
                            exit(1);
                        }
                    }

                    // Close all pipe ends except the ones being used by this command
                    for (int j = 0; j < num_pipe; j++)
                    {
                        if (i > 0 && j == i - 1)
                        {
                            close(pipes[j][1]);
                        }
                        else if (i < num_pipe && j == i)
                        {
                            close(pipes[j][0]);
                        }
                        else
                        {
                            close(pipes[j][0]);
                            close(pipes[j][1]);
                        }
                    }
                    execute_pipe_commamd(commands[i], input_files[i], output_file[i], append_flag[i]);
				}
			}
			for (int i = 0; i < num_pipe; i++)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
			}
			int status;
			for (int i = 0; i < num_commands; i++)
			{
				wait(&status);
			}
		}

		
		// clean up commands
		for (i = 0; i < num_commands; i++)
		{
			int j;
			for (j = 0; j < arry_arg[i]; j++)
			{
				commands[i][j] = NULL;
				//printf("clean commands[%d][%d] = %s \n", i, j, commands[i][j]);
			}
		}
		dup2(orig_stdin, STDIN_FILENO);	  // reset stdin to its original state
		dup2(orig_stdout, STDOUT_FILENO); // reset stdout to its original state
	}
}

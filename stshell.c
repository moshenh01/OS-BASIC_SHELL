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

pid_t child_pid = 0;
char *history[100];
int history_length = 0;
int current_index = 0;
int history_index = 0;
int history_flag = 0;

void execute_command(char **command, char *input_file, char *output_file, int append, int pipefd[2])
{
	pid_t pid;
	int status;
	// printf("1)Executing command: %s %s %s %d %d  \n", command[0], command[1], command[2], pipefd[0], pipefd[1]);
	// printf("1)Executing command: %s %s %s %s %s %d %d  \n", command[0], command[1], command[2],command[3],command[4], pipefd[0], pipefd[1]);
	
	pid = fork();
	child_pid = pid;

	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		// Child process
		signal(SIGINT, SIG_DFL);
		//use pipe if necessary
		if (pipefd[0] != -1 && pipefd[1] != -1)
		{

			
			close(pipefd[0]);
			if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 failed");
				exit(EXIT_FAILURE);
			}
			close(pipefd[1]);
		}

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
		
		//use pipe if necessary
		if (pipefd[0] != -1 && pipefd[1] != -1)
		{
			close(pipefd[1]);
			if (dup2(pipefd[0], STDIN_FILENO) == -1)
			{
				perror("dup2 failed");
				exit(EXIT_FAILURE);
			}
			close(pipefd[0]);
		}

		
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
		for (int j = 0; j < arry_arg[i] ; j++)
		{
			printf("%s ", commands[i][j]);
		}
		printf("\n");
	}
}

void add_command_to_history(char *commands[10][10], int num_commands, int arry_arg[])
{
	if(commands[0][0] == NULL || commands[0] == NULL || strcmp(commands[0][0], "") == 0)
		return;

	char *command = (char *)calloc(1024, sizeof(char));
	for (int i = 0; i < num_commands; i++)
	{
		for (int j = 0; j < arry_arg[i]; j++)
		{	

			strcat(command, commands[i][j]);
			strcat(command, " ");
		}
		if(i != num_commands - 1)
			strcat(command, "| ");
	}
	strcat(command, "\0");
	history[history_index] = (char *)malloc(strlen(command) + 1);
	//printf("command = %s \n", command);
	strcpy(history[history_index], command);
	history_index = (history_index +1)%100;

	if(history_index == 100)
		history_flag = 1;

	if(history_flag == 1)
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
	for (int i = 0; i < history_index; i++)
	{
		printf("%d: %s\n", i, history[i]);
	}
}


int main()
{

	char input_stream[1024];
	char *commands[10][10];
	char *token;
	char *input_file = NULL;
	char *output_file = NULL;
	int append = 0;		  // 0: overwrite, 1: append
	int num_commands = 0; // number of commands.
	int num_args = 0;	  // number of arguments for the command.
	int pipefd[2];		  // file descriptors for pipe
	int orig_stdin = dup(STDIN_FILENO); // save original stdin file descriptor
    int orig_stdout = dup(STDOUT_FILENO); // save original stdout file descriptor
	int arry_arg[10]; // number of arguments for each command
	
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
		num_commands = 0;
		num_args = 0;
		token = strtok(input_stream, " ");
		while (token != NULL)
		{
			if (strcmp(token, ">") == 0)
			{
				// Output redirection, catch the next token as output file
				token = strtok(NULL, " ");
				output_file = token;
				token = strtok(NULL, " ");
				append = 0;
			}
			else if (strcmp(token, ">>") == 0)
			{
				// Output redirection, catch the next token as output file
				token = strtok(NULL, " ");
				output_file = token;
				token = strtok(NULL, " ");
				append = 1;
			}
			else if (strcmp(token, "<") == 0)
			{
				// Input redirection, catch the next token as input file
				token = strtok(NULL, " ");
				input_file = token;
				token = strtok(NULL, " ");
			}
			else if (strcmp(token, "|") == 0)
			{
				// Pipe, from new its a new command
				arry_arg[num_commands-1] = num_args;
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
		arry_arg[num_commands-1] = num_args;

		commands[num_commands - 1][num_args] = NULL; // NULL terminate the last command and argument
		//print_arry_arg(arry_arg, num_commands);
		//print_commands(commands, num_commands, arry_arg);
		add_command_to_history(commands, num_commands, arry_arg);
		
		/* Is command empty or ..*/
		if (commands[0] == NULL || commands[0][0] == NULL || strcmp(commands[0][0], "") == 0 || strcmp(commands[0][0], "\n") == 0)
			continue;

		if (strcmp(commands[0][0], "exit") == 0){
			free_history();
			exit(0);
		}

		int i;
		for (i = 0; i < num_commands; i++)
		{
			
			if (i < num_commands - 1)
			{
				// Not the last command, create pipe
				if (pipe(pipefd) == -1)
				{
					perror("pipe failed");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				// Last command, no pipe
				pipefd[0] = -1;
				pipefd[1] = -1;
				
			}
			
			// execute cmp tool if user types "cmp"
			if (strcmp(commands[i][0], "cmp") == 0)
			{

				if (arry_arg[i] > 4 || arry_arg[i]  < 3)
				{
					printf("Usage: cmp file1 file2 -i/-v\n");
					continue;
				}
				commands[i][0] = "./cmp";
				execute_command(commands[i], input_file, output_file, append, pipefd);
			}
			// execute copy tool if user types "copy"
			else if (strcmp(commands[i][0], "copy") == 0)
			{
				if (arry_arg[i]  > 4 || arry_arg[i]  < 3)
				{
					printf("Usage: copy file1 file2 -v\n");
					continue;
				}
				commands[i][0] = "./copy";
				execute_command(commands[i], input_file, output_file, append, pipefd);
			}


		
		

			else if (strcmp(commands[i][0], "encode") == 0)
			{
				if (arry_arg[i] > 3 || arry_arg[i]  < 2 )
				{
					printf("Usage: encode <codec> <message>\n");
					continue;
				}

				commands[i][0] = "./encode";
				execute_command(commands[i], input_file, output_file, append, pipefd);
			}
			else if (strcmp(commands[i][0], "decode") == 0)
			{
				if (arry_arg[i] > 3 || arry_arg[i]  < 2)
				{
					printf("Usage: decode <codec> <message>\n");
					continue;
				}

				commands[i][0] = "./decode";
				execute_command(commands[i], input_file, output_file, append, pipefd);
			}
			else if(strcmp(commands[i][0] ,"history") == 0){
				if (arry_arg[i] > 1)
				{
					printf("Usage: history\n");
					continue;
				}
				else
				{
					print_history();
				}
			}
			else
			{
				// Execute command
				//printf("Executing command: %s\n", commands[i][0]);
				execute_command(commands[i], input_file, output_file, append, pipefd);

			}

		}
		//clean up commands
		for (i = 0; i < num_commands; i++)
		{
			int j;
			for (j = 0; j < arry_arg[i]; j++)
			{
				commands[i][j] = NULL;
			}
		}

		dup2(orig_stdin, STDIN_FILENO); // reset stdin to its original state
    	dup2(orig_stdout, STDOUT_FILENO); // reset stdout to its original state
		
	}
}

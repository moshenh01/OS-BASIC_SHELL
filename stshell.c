#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>

pid_t child_pid = 0;

void execute_command(char **command, char *input_file, char *output_file, int append)
{
	pid_t pid;
	int status;

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
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid failed");
			exit(EXIT_FAILURE);
		}

		if (WIFEXITED(status))
		{
			printf("%s+ returned %d\n",command[0], WEXITSTATUS(status));
		}
		else
		{
			printf("%s+ exited abnormally\n" ,command[0]);
		}
	}
}

void sig_hendler(int signum)
{
	if (signum == SIGINT)
	{
		// printf("ignore ctrl+c \n");
		// ignore ctrl+c for the parent process
		if (child_pid != 0)
		{
			kill(child_pid, SIGINT);
		}
	}
	if (signum == SIGUSR1)
	{
		// do something
	}
}

int main()
{
	int i;
	char *argv[10];
	char command[1024];
	char *token;
	char *input_file = NULL;
    char *output_file = NULL;

	// ignore ctrl+c
	if (signal(SIGINT, sig_hendler) == SIG_ERR)
	{
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}

	while (1)
	{
		printf("myshell> ");
		fgets(command, 1024, stdin);
		command[strlen(command) - 1] = '\0'; // replace \n with \0

		/* parse command line */
		i = 0;
		token = strtok(command, " ");
		while (token != NULL)
		{
			argv[i] = token;
			token = strtok(NULL, " ");
			i++;
		}
		argv[i] = NULL; // last element of argv must be NULL for execvp to work properly.

		/* Is command empty or ..*/
		if (argv[0] == NULL || strcmp(argv[0], "") == 0 || strcmp(argv[0], "\n") == 0)
			continue;

		if (strcmp(argv[0], "exit") == 0)
			exit(0);
		

		
		// execute cmp tool if user types "cmp"
		if (strcmp(argv[0], "cmp") == 0)
		{
			if (i > 4 || i < 3)
			{
				printf("Usage: cmp file1 file2 -i/-v\n");
				continue;
			}
			argv[0] = "./cmp";
			execute_command(argv, NULL, NULL, 0);
			
		}
		// execute copy tool if user types "copy"
		if (strcmp(argv[0], "copy") == 0)
		{
			if (i > 4 || i < 3)
			{
				printf("Usage: copy file1 file2 -v\n");
				continue;
			}
			argv[0] = "./copy";
			execute_command(argv, NULL, NULL, 0);
		}
		
		if (strcmp(argv[0], "encode") == 0)
		{
			if (i != 3)
			{
				printf("Usage: encode <codec> <message>\n");
				continue;
			}

			argv[0] = "./encode";
			execute_command(argv, NULL, NULL, 0);

		}
		if (strcmp(argv[0], "decode") == 0)
		{
			if (i != 3)
			{
				printf("Usage: decode <codec> <message>\n");
				continue;
			}

			argv[0] = "./decode";
			execute_command(argv, NULL, NULL, 0);
		}
	}
}

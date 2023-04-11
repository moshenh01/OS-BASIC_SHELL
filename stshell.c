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

void sig_hendler(int signum) {
	if (signum == SIGINT) {
		//printf("ignore ctrl+c \n");
		//ignore ctrl+c for the parent process
		if (child_pid != 0) {
        	kill(child_pid, SIGINT);
    	}
	}
	if(signum == SIGUSR1) {
		//do something
	}
}
	

int main() {
	int i;
	char *argv[10];
	char command[1024];
	char *token;
//ignore ctrl+c 
if(signal(SIGINT, sig_hendler) == SIG_ERR) {
	printf("Error: %s\n", strerror(errno));
	exit(1);
}

	while (1) {
	    printf("myshell> ");
	    fgets(command, 1024, stdin);
	    command[strlen(command) - 1] = '\0'; // replace \n with \0

	    /* parse command line */
	    i = 0;
	    token = strtok (command," ");
	    while (token != NULL)
	    {
		argv[i] = token;
		token = strtok (NULL, " ");
		i++;
	    }
		
	    argv[i] = NULL;// last element of argv must be NULL for execvp to work properly.
 
	        
		/* Is command empty or ..*/  
	    if (argv[0] == NULL || strcmp(argv[0], "") == 0 ||strcmp(argv[0], "\n") == 0)
			continue;

		if (strcmp(argv[0], "exit") == 0) {
            exit(0);
        }




		//
	    // /* for commands not part of the shell command language */ 
	    // if (fork() == 0) { 
		// execvp(argv[0], argv);
		// wait(NULL);// wait for child process to finish executing before continuing to next iteration of while loop.
	    // }   
		 // execute cmp tool if user types "cmp"
        if (strcmp(argv[0], "cmp") == 0) {
			 if (i > 4 || i < 3) {
                printf("Usage: cmp file1 file2 -i\n");
                continue;
            }
			argv[0] = "./cmp";
            pid_t pid = fork();
			child_pid = pid;
			
            if (pid == 0) {
                execvp(argv[0], argv);
                printf("Error: could not execute command\n");
                exit(1);
            } else if (pid < 0) {
                printf("Error: could not fork\n");
                exit(1);
            } else {
                int status;
                waitpid(pid, &status, 0);
				 if (WIFEXITED(status)) {
                    printf("./cmp returned %d\n", WEXITSTATUS(status));
                }
				else {
                    printf("./cmp exited abnormally\n");
                }
            }
        }
		if(strcmp(argv[0], "copy") == 0){
			if (i > 4 || i < 3) {
				printf("Usage: copy file1 file2 -v\n");
				continue;
			}
			argv[0] = "./copy";
			pid_t pid = fork();
			child_pid = pid;
			
			if (pid == 0) {
				execvp(argv[0], argv);
				printf("Error: could not execute command\n");
				exit(1);
			} else if (pid < 0) {
				printf("Error: could not fork\n");
				exit(1);
			} else {
				int status;
				waitpid(pid, &status, 0);
				 if (WIFEXITED(status)) {
					printf("./copy returned %d\n", WEXITSTATUS(status));
				}
				else {
					printf("./copy exited abnormally\n");
				}
			}
		}

		if (strcmp(argv[0], "encode") == 0 ) {
    		if (i != 3) {
				printf("Usage: encode <codec> <message>\n");
				continue;
			}
			
			argv[0] = "./encode";
			pid_t pid = fork();
			child_pid = pid;
			
			if (pid == 0) {
				//printf("execvp(%s, %s, %s, %s, %s)\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
				execvp(argv[0], argv);
				printf("Error: could not execute command\n");
				exit(1);
			} else if (pid < 0) {
				printf("Error: could not fork\n");
				exit(1);
			} else {
				int status;
				waitpid(pid, &status, 0);
				if (WIFEXITED(status)) {
					printf("./encode returned %d\n", WEXITSTATUS(status));
				} else {
					printf("./encode exited abnormally\n");
				}
			}
		
		}
		if (strcmp(argv[0], "decode") == 0 ) {
			if (i != 3) {
				printf("Usage: decode <codec> <message>\n");
				continue;
			}

			argv[0] = "./decode";
			pid_t pid = fork();
			child_pid = pid;

			if (pid == 0) {
				
				execvp(argv[0], argv);
				printf("Error: could not execute command\n");
				exit(1);
			} else if (pid < 0) {
				printf("Error: could not fork\n");
				exit(1);
			} else {
				int status;
				waitpid(pid, &status, 0);
				if (WIFEXITED(status)) {
					printf("./decode returned %d\n", WEXITSTATUS(status));
				} else {
					printf("./decode exited abnormally\n");
				}

			}
		}

    } 

	
}

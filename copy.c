#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

//the tool will copy a file to another place and/or name.
// The tool will return “0” on success, or “1” on failure (return an INT)
// The tool will create a new file, if it does not exist, but it will not overwrite a file if it do exist.
// the tool will support -v flag, that will output “success” if the file is copied, or “target file exist” if this is
// the case, or “general failure” on some other problem (in addition to the returned INT value).
// The tool will support -f flag (that means force), that allows to overwrite the target file
//"usage copy <file1> <file2> -v"

int main(int argc, char *argv[]){
  
    //printf("argv[0] = %s    argv[1] = %s    argv[2] = %s        argc = %d \n" , argv[0], argv[1], argv[2], argc);
    FILE *fp1 = fopen(argv[1], "rb");
    FILE *fp2 = fopen(argv[2], "rb");
    if (fp1 == NULL) {
        printf("Error: cannot open file \n");
        exit(1);
    }
    if (argc == 3)
    {
        goto end;
    }
    
    //if argv[2] == -v print "success" or "target file exist" or "general failure"
    if (strcmp(argv[3], "-v") == 0) {
        if(fp2 != NULL) {
            printf("target file exist \n");
            exit(1);
        }
        fp2 = fopen(argv[2], "w");
        if (fp2 == NULL) {
            printf("general failure \n");
            exit(1);
        }
        int c1;
        while (1) {
            c1 = fgetc(fp1);
            if (c1 == EOF) {
                break;
            }
            fputc(c1, fp2);
        }
        printf("success \n");
        exit(0);
    }
    //if argv[2] == -f overwrite the target file
    else if (strcmp(argv[3], "-f") == 0) {
        fp2 = fopen(argv[2], "wb");
        if (fp2 == NULL) {
            printf("general failure \n");
            exit(1);
        }
        int c1;
        while (1) {
            c1 = fgetc(fp1);
            if (c1 == EOF) {
                break;
            }
            fputc(c1, fp2);
        }
        exit(0);
    }
    else {
        end:
        if (fp2 != NULL) {
            //printf("target file exist \n");
            exit(1);
        }
        fp2 = fopen(argv[2], "wb");
        if (fp2 == NULL) {
            //printf("general failure \n");
            exit(1);
        }
        int c1;
        while (1) {
            c1 = fgetc(fp1);
            if (c1 == EOF) {
                break;
            }
            fputc(c1, fp2);
        }
        exit(0);
    
    }
     


}

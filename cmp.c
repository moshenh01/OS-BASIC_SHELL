#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

//the tool will compare two files, and return “0” if they are equal, and “1” if not (return an INT).
// The tool will support -v flag for verbose output. By this we mean that the tool will print “equal” or
// “distinct”, in addition to returning the int value.
// The toll will support -i flag, that mean “ignore case” so “AAA” and “aaa” meaning equals
int main(int argc, char *argv[]) {
   
    
   // printf("argv[0] = %s    argv[1] = %s    argv[2] = %s        argc = %d \n" , argv[0], argv[1], argv[2], argc);  
    FILE *fp1 = fopen(argv[1], "rb");
    FILE *fp2 = fopen(argv[2], "rb");
    if (fp1 == NULL || fp2 == NULL) {
        printf("Error: cannot open file \n");
        exit(1);
    }
    if (argc == 3)
    {
        goto end;
    }
    
    
    //if argv[3] == -v print "equal" or "distinct" 
    if (strcmp(argv[3], "-v") == 0) {
        
        int c1, c2;
        while (1) {
            c1 = fgetc(fp1);
            c2 = fgetc(fp2);
            if (c1 != c2) {
                printf("distinct \n");
                exit(1);
            }
            if ((c1 == EOF && c2 != EOF) || (c1 != EOF && c2 == EOF)) {

                printf("distinct \n");
                exit(1);
            }
            if (c1 == EOF && c2 == EOF)
            {
                break;
            }
        }
        printf("equal \n");
        exit(0);
    }
    
    //if argv[3] == -i ignore case
    else if (strcmp(argv[3], "-i") == 0) {
        int c1, c2;
        while (1) {
            c1 = fgetc(fp1);
            c2 = fgetc(fp2);
            if (c1 != c2) {
                if (c1 >= 'A' && c1 <= 'Z') {
                    c1 += 32;
                }
                if (c2 >= 'A' && c2 <= 'Z') {
                    c2 += 32;
                }
                if (c1 != c2) {
                    exit(1);
                }
            }
            if ((c1 == EOF && c2 != EOF) || (c1 != EOF && c2 == EOF)) {
                exit(1);
            }
            if (c1 == EOF && c2 == EOF)
            {
                break;
            }
        }
        //printf("Files are equal \n");
        exit(0);
    }
    else{
        end:
        int c1, c2;
        while (1) {
            c1 = fgetc(fp1);
            c2 = fgetc(fp2);
            if (c1 != c2) {
                exit(1);
            }
            
            if ((c1 == EOF && c2 != EOF ) || (c1 != EOF && c2 == EOF)) {
                exit(1);
            }
            if (c1 == EOF && c2 == EOF)
            {
                break;
            }
            
        }
        printf("Files are equal \n");
        exit(0);
    }


}

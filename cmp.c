#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

//func that say if argv containe -v or -f


//the tool will compare two files, and return “0” if they are equal, and “1” if not (return an INT).
// The tool will support -v flag for verbose output. By this we mean that the tool will print “equal” or
// “distinct”, in addition to returning the int value.
// The toll will support -i flag, that mean “ignore case” so “AAA” and “aaa” meaning equals

int main(int argc, char *argv[]) {

    if(argc < 3 || argc > 5)
    {
        invalid:
        printf("Usage: cmp file1 file2 [-v] [-i] \n");
        exit(1);
    }
   
    //sleep(10);
    //printf("argv[0] = %s    argv[1] = %s    argv[2] = %s        argc = %d \n" , argv[0], argv[1], argv[2], argc);  
    FILE *fp1 = fopen(argv[1], "rb");
    FILE *fp2 = fopen(argv[2], "rb");
    if (fp1 == NULL || fp2 == NULL) {
        printf("Error: cannot open file \n");
        exit(1);
    }
   
    
    if(argv[3] == NULL){
        
        int c1, c2;
        while (1) {
            c1 = fgetc(fp1);
            c2 = fgetc(fp2);
            if (c1 != c2) {
                fclose(fp1);
                fclose(fp2);
                exit(1);
            }
            
            if ((c1 == EOF && c2 != EOF ) || (c1 != EOF && c2 == EOF)) {
                fclose(fp1);
                fclose(fp2);
                exit(1);
            }
            if (c1 == EOF && c2 == EOF)
            {
                break;
            }
            
        }
        //printf("Files are equal \n");
        fclose(fp1);
        fclose(fp2);
        exit(0);
    }
    //if argv[3] == -v print "equal" or "distinct" 
    else if ((strcmp(argv[3], "-v") == 0) && (argv[4] == NULL)) {
        
        int c1, c2;
        while (1) {
            c1 = fgetc(fp1);
            c2 = fgetc(fp2);
            if (c1 != c2) {
                printf("distinct \n");
                fclose(fp1);
                fclose(fp2);
                exit(1);
            }
            if ((c1 == EOF && c2 != EOF) || (c1 != EOF && c2 == EOF)) {

                printf("distinct \n");
                fclose(fp1);
                fclose(fp2);
                exit(1);
            }
            if (c1 == EOF && c2 == EOF)
            {
                break;
            }
        }
        printf("equal \n");
        fclose(fp1);
        fclose(fp2);
        exit(0);
    }
    
    //if argv[3] == -i ignore case
    else if ((strcmp(argv[3], "-i") == 0) && (argv[4] == NULL)) {
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
                    fclose(fp1);
                    fclose(fp2);
                    exit(1);
                }
            }
            if ((c1 == EOF && c2 != EOF) || (c1 != EOF && c2 == EOF)) {
                fclose(fp1);
                fclose(fp2);
                exit(1);
            }
            if (c1 == EOF && c2 == EOF)
            {
                break;
            }
        }
        //printf("Files are equal \n");
        fclose(fp1);
        fclose(fp2);
        exit(0);
    }
    else if(( (strcmp(argv[3], "-i") == 0) && (strcmp(argv[4], "-v") == 0) ) ||
            ( (strcmp(argv[3], "-v") == 0) && (strcmp(argv[4], "-i") == 0) ) )
    {
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
                    printf("distinct \n");
                    fclose(fp1);
                    fclose(fp2);
                    exit(1);
                }
            }
            if ((c1 == EOF && c2 != EOF) || (c1 != EOF && c2 == EOF)) {
                printf("distinct \n");
                fclose(fp1);
                fclose(fp2);
                exit(1);
            }
            if (c1 == EOF && c2 == EOF)
            {
                break;
            }
        }
        printf("equal \n");
        fclose(fp1);
        fclose(fp2);
        exit(0);
    }

    
    else
    {
        fclose(fp1);
        fclose(fp2);
        goto invalid;
       
    }
 

}

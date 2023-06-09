#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "codec.h"


int main(int argc, char *argv[]) {
    

    if (argc < 2 || argc > 3)
    {
      printf("Usage: decode codec [message]\n");
      exit(1);
    }
  
    
    char *codec = argv[1];
    char *message = argv[2];
    if (argc == 2)
    {
      message = malloc(100);
      fgets(message, 100, stdin);
      message[strlen(message) - 1] = '\0';
    }
    
    
    
   
    if (strcmp(codec, "codecA") == 0) {
      void* hendle = dlopen("./codecA", 1);
      if(hendle == NULL) {
        printf("Error: %s\n", dlerror());
        exit(1);
      }
      decode_func decode = dlsym(hendle, "decode");
      if(decode == NULL) {
        printf("Error: %s\n", dlerror());
        exit(1);
      }
      decode(message);
      dlclose(hendle);

    }
    else if (strcmp(codec, "codecB") == 0) {
      void* hendle = dlopen("./codecB", 1);
      if (hendle == NULL)
      {
        printf("Error: %s\n", dlerror());
        exit(1);
      }
      
      decode_func decode = dlsym(hendle, "decode");
      if(decode == NULL) {
        printf("Error: %s\n", dlerror());
        exit(1);
      }
      decode(message);
      dlclose(hendle);
    }
    else {
      printf("Invalid codec: %s\n", codec);
      exit(1);
    }

    printf("%s\n", message);
    if (argc == 2)
    {
      free(message);
    }
    

  return 0;
}

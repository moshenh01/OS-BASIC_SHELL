#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "codec.h"

int main(int argc, char *argv[]) {
    
    char *codec = argv[1];
    char *message = argv[2];
   
    
    
    
    if (strcmp(codec, "codecA") == 0) {
      void* hendle = dlopen("./codecA", 1);//1 means RTLD_LAZY, dlopen will load the library and return a handle to it 
                                        //RTLD_LAZY means that the symbols in the library are resolved when the library is loaded
      if(hendle == NULL) {
        printf("Error: %s\n", dlerror());
        exit(1);
      }                              
      encode_func encode = dlsym(hendle, "encode");//dlsym will return the address of the symbol in the library
      if(encode == NULL){                          //encode_func is a pointer to a function that takes a char* and returns void
                                                  //dlsym will return the address of the function encode
        printf("Error: %s\n", dlerror());
        exit(1);
      }                                          
      encode(message);
      dlclose(hendle);//close the library we need to close the library when we are done with it to free up memory and resources 
                      //dlclose will close the library and return 0 if successful

    } else if (strcmp(codec, "codecB") == 0) {
      void* hendle = dlopen("./codecB", 1);
      if (hendle == NULL)
      {
        printf("Error: %s\n", dlerror());
        exit(1);
      }
      
      encode_func encode = dlsym(hendle, "encode");
      if(encode == NULL) {
        printf("Error: %s\n", dlerror());
        exit(1);
      }
      encode(message);
      dlclose(hendle);
    }
    else {
    printf("Invalid codec: %s\n", codec);
    exit(1);
    }

    printf("%s\n", message);

  return 0;
}

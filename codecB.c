#include <string.h>
#include "codec.h"


void encode(char *message) {
  for (int i = 0; i < strlen(message); i++) {
        if(message[i] < 122 && message[i] > 96 ){
            if(message[i] == 120 || message[i] == 121 || message[i] == 122)
                message[i] = message[i] - 23;
            else
                message[i] = message[i] + 3;
        }
        else if(message[i] < 90 && message[i] > 64){
            if(message[i] == 88 || message[i] == 89 || message[i] == 90)
                message[i] = message[i] - 23;
            else
                message[i] = message[i] + 3;
        }
  }
}

void decode(char *message) {
  for (int i = 0; i < strlen(message); i++) {
    if (message[i] < 122 && message[i] > 96)
    {
        if(message[i] == 97 || message[i] == 98 || message[i] == 99)
            message[i] = message[i] + 23;
        else
            message[i] = message[i] - 3;
    }
    else if (message[i] < 90 && message[i] > 64)
    {
        if(message[i] == 65 || message[i] == 66 || message[i] == 67)
            message[i] = message[i] + 23;
        else
            message[i] = message[i] - 3;
    }
  }
}


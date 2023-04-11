#include <string.h>
#include <ctype.h>
#include "codec.h"


void encode(char *message) {
  for (int i = 0; i < strlen(message); i++) {
    if (islower(message[i])) {
      message[i] = toupper(message[i]);
    } else if (isupper(message[i])) {
      message[i] = tolower(message[i]);
    }
  }
}

void decode(char *message) {
  encode(message);
}


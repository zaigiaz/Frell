#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LENGTH 256

enum commands {
  pwd,
  cd,
  ls,
  touch,
};

void interpret_input(char* input) {}

// TODO: add system calls for getting files and directories and stuff like that
// TODO: fix extra newline being created somehow
// TODO: 

int main() {

  char *input = malloc(sizeof(char) * LENGTH);

  printf("\nWelcome to Frell: Friendly Shell!\n");
  printf("-----------------------------------\n");
  
  while(true) {

    printf("\n[user@shell] ");

    // await input
    scanf("%s", input);

    if(*input == 'q') {
      perror("\nquitting shell\n");
      break;
    }

    // interpret input
    interpret_input(input);

    // flush buffer
    size_t len = sizeof(input);
    memset(input, 0x00, len);
  }

  free(input);

  return 0;
}


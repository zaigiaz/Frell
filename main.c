#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LENGTH 128
#define SIG_NO_CUR_DIR 1

// TODO :: execute commands as child process or add my own
// TODO :: add command line flag parsing

int main(int argc, char** argv) {

  char input[LENGTH];

  printf("-----------------------------------");
  printf("\nWelcome to Frell: Friendly Shell!\n");
  printf("      Commands: quit, help         \n");
  printf("-----------------------------------\n");
  
  while(true) {

    char buffer[LENGTH];
    if(getcwd(buffer, LENGTH) == NULL) {
      perror("failed to get current directory");
      exit(SIG_NO_CUR_DIR);
    }

    // print listing
    printf("[%s ] ", buffer);

    // read input
    if(fgets(input,LENGTH,stdin) == NULL) {
      perror("failed to get standard input");
      exit(1);
    }

    // strip newline from fgets function
    size_t length = strlen(input);
    if(length > 0 && input[length-1] == '\n') { 
      input[length-1] = '\0'; 
    }

    // quit action
    if(strcmp(input, "quit") == 0) {
      perror("\nquitting shell\n");
      exit(1);
    }

    // help action
    if(strcmp(input, "help") == 0) {
      printf("\nCurrent shell works with basic UNIX commands, through execvp() function\n");
    }

    // flush buffer
    size_t len = sizeof(input);
    memset(input, 0x00, len);
  }


  return 0;
}




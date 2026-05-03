#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

#define LENGTH 256
#define SIG_NO_CUR_DIR 1

// TOOD :: use strtok() method to split flags and commands
// TODO :: use execvp to handle things
// TODO :: implement signals from gist to handle diff program outputs
// TODO :: implement jobs and job handling

int main() {

  char input[LENGTH];

  printf("------------------------------------------------\n");
  printf("      Welcome to Frell: Friendly Shell!\n       ");
  printf("      Commands: Frell-Quit, Frell-Help         \n");
  printf("-----------------------------------------------\n");
  
  // Main loop of program
  while(true) {

    char buffer[LENGTH];

    if(getcwd(buffer, LENGTH) == NULL) {
      perror("failed to get current directory");
      exit(SIG_NO_CUR_DIR);
    }

    // print listing
    printf("[%s ] ", buffer);

    // read input
    if(fgets(input, LENGTH, stdin) == NULL) {
      perror("failed to get standard input");
      exit(1);
    }

    // strip newline from fgets function
    size_t length = strlen(input);
    if(length > 0 && input[length-1] == '\n') { 
      input[length-1] = '\0'; 
    }

    // quit action
    if(strcmp(input, "Frell-Quit") == 0) {
      perror("\nquitting shell\n");
      exit(1);
    }

    // help action
    if(strcmp(input, "Frell-Help") == 0) {
      printf("\nCurrent shell works with basic UNIX commands, through execvp() function\n");
    }

    // flush buffer
    size_t len = sizeof(input);
    
    memset(input, 0x00, len);
  }


  return 0;
}




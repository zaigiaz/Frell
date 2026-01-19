#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LENGTH 64

// TODO: execute commands as child process
// TODO: add command line flag parsing

int main() {

  char input[LENGTH];

  printf("-----------------------------------");
  printf("\nWelcome to Frell: Friendly Shell!\n");
  printf("      Commands: quit, help         \n");
  printf("-----------------------------------\n");
  
  while(true) {

    char buffer[LENGTH];
    if(getcwd(buffer, LENGTH) == NULL) {
      perror("failed to get current directory");
      exit(1);
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

    // TODO: tokenize input based on spaces
    /* strtok(input, " "); */

    
    // quit action
    if(strcmp(input, "quit") == 0) {
      perror("\nquitting shell\n");
      exit(1);
    }

    // help action
    if(strcmp(input, "help") == 0) {
      printf("\nCurrent shell works with basic UNIX commands, through execvp() function\n");
    }


    // interpret input
    /* pid_t pid = fork(); */
    /* if(pid == 0) { */
    /*   execvp(args[0], args); */
    /*   perror("exec failed"); */
    /*   exit(1); */
    /* } */
    /* else if (pid > 1) { */
      
    /* } */


    // flush buffer
    size_t len = sizeof(input);
    memset(input, 0x00, len);
  }


  return 0;
}


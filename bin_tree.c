// AUTHOR: Zaigiaz
// START DATE: August 2026

#include <stdio.h>

enum Tokens {
  PIPE,
  INPUT_REDIR,
  AMP
};

// values for each node
typedef struct {
  size_t val;
  const char* name;
};

// actual node for making binary tree
typedef struct {
  struct node *left, *right;
  n_data* data;
} node;


int main(void) {
  printf("hello\n"); 
  n_data data;

  data.val = 5;
  data.name = "jonathan";
  printf("%s is %zu years old", data.name, data.val);
}


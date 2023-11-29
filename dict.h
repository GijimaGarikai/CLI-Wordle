#pragma once
#include <stdio.h>

// useful helper function
char* make_copy(const char *list, int len);

// dict struct declaration
typedef struct{
    int size;
    int capacity;
    char ** words;
  } dict;

dict * make_dict(void);

void dict_add(dict * thisDict, char * word);

void fill_dict(dict * myDict, FILE *input);

void free_dict(dict * thisDict);



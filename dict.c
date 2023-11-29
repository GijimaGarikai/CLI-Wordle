#include "dict.h"
#include <stdio.h>
#include <stdlib.h>

// chose 5 since wordle words are 5 letters
#define LEN 5

/* make_copy
PreConditions:
*list - pointer to character list to be copied
len - number of elements to be copied

PostConditions:
Returns heap allocated copy of list
*/
char* make_copy(const char *list, int len) {
  // saves space for len+1 chars to account for \0 character
  char * dest = malloc((len+1) * sizeof(char));
  for (int i = 0; i < len; i++) {
    dest[i] = list[i];
  }
  dest[len] = '\0'; 
  return dest;
}

/* make_dict
PreConditions:
None

PostConditions:
returns a heap allocated dict struct with heap allocated words subfield to hold pointers to heap allocated words
*/
dict * make_dict(void) {
  dict * list = malloc(1 * sizeof(dict));
  list->words = malloc(10 * sizeof(char*)); // randomly chose 10
  list->size = 0;
  list->capacity = 10;
  return list;
}

/* dict_add
PreConditions:
*thisDict - pointer to dict being added to
*word - word being added to thisDict

PostConditions:
adds word to dict and updates subfields of dict
*/
void dict_add(dict * thisDict, char * word) {
  int index, maxcap;
  index = thisDict->size;
  maxcap = thisDict->capacity;
  // check if thisDict is full
  // if full, double capacity and move contents to new dictionary
  if (index >= maxcap) {
    int newcap = 2*maxcap;
    // allocate new dict
    char ** newDict = malloc(newcap * sizeof(char*));
    // copy each pointer in old dict to new dict
    for (int i = 0; i < index; i++) {
      newDict[i] = thisDict->words[i];
    }
    // remove pointer to old dict
    free (thisDict->words);
    // update pointer to new dict
    thisDict->words = newDict;
    thisDict->capacity = newcap;
  }
  // add word to end of new dict and update size
  thisDict->words[index] = word;
  thisDict->size = index+1;
}

/* fill_dict
PreConditions:
*myDict - dict to be filled with words
*input - file to read words from, assumed to be 5 letter words separated by
         newline character
PostConditions:
fills myDict with all 5 letter words in input file
*/
void fill_dict(dict * myDict, FILE *input) {
  char curchar;
  curchar = getc(input);
  char curword[LEN+1];
  int index = 0;
  // while we aren't at the end of the input file
  while (curchar != EOF) {
    // if we encounter newline then store terminating character and add word to dict
    if (curchar == '\n') {
      curword[index] = '\0';
      dict_add(myDict, make_copy(curword, LEN));
    }
    else {
      // otherwise just add character to word we are building
      curword[index] = curchar;
    }
    index++;
    index = index % 6; // keep index in range
    curchar = getc(input);
  }
}
/* free_dict
PreConditions:
*thisDict - dict struct to be freed
PostConditions:
frees all dynamically allocated memory used by thisDict
*/
void free_dict(dict * thisDict) {
  int len;
  len = thisDict->size;
  for (int i = 0; i < len; i++) {
    // frees each heap allocated string in the dictionary
    free(thisDict->words[i]);
  }
  // free the pointer to the strings
  free(thisDict->words);
  // free the dict itself
  free(thisDict);
}
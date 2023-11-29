 /***********************************************************************
     * Name(s)  (Garikai)               *
     * Assignment name (Wordle)                                      *
     *                                                                     *
     * Due Date 11/22/23                                                     *
     ***********************************************************************/

    /* *********************************************************************
     * Academic honesty certification:                                     *
     *   Written/online sources used:                                      *
     *      homeowrk materials                              *
     *   Help obtained                                                     *
     *     none                   *
     *     ["none" required for Homework Problems                          *
     *   My/our typed name(s) below confirms that the above list of sources *
     *   is complete AND that I/we have not talked to anyone else          *
     *   (e.g., CSC 161 students, tutors) about the solutions              *
     *                                                                     *
     *   Signature: Garikai                                                      *
     ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <errno.h>

// added feature 1 into get_guess and feature 2 into new summary_report function
// added input check in get_guess to make sure all input is lowercase English letters

// chose 5 since wordle words are 5 letters
#define LEN 5
// 12972 was chosen after running a counting function on the words5 dictionary
#define DICT_LEN 12972

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

// dict struct declaration
typedef struct{
    int size;
    int capacity;
    char ** words;
  } dict;

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

/* clear_stdin
PreConditions:
input - the most recent character taken using getchar or scanf

PostConditions:
cycles through user input on stdin until the end of the input, newline character, 
is encountered. This ensures that when getchar is called again the user will input new 
data rather than getchar taking whatever was left "unused" from last stdin input
*/
void clear_stdin(char input) {
  while (input != '\n') {
    input = getchar();
  }
}

/* get_guess
PreConditions:
turn - integer indicating which turn is being played
*guess - pointer to where player's guess word will be stored

PostConditions:
Returns void but stores player input in *guess and outputs into player
letting them know the turn and guess input
*/
void get_guess(const int turn, char *guess) {
  printf("Please enter your 5 letter word!\n");
  int char_count = 0;
  char letter;
  while (char_count < 6) {
    // read and store stdin input until newline character encountered
    letter = getchar();
    if (letter == '\n' && char_count != 5) {
      printf("Invalid input - too short, please retry.\n");
      char_count = 0;
    }
    else if (letter != '\n' && char_count == 5) {
      printf("Invalid input - too long, please retry.\n");
      char_count = 0;
      clear_stdin(letter);
    }//ensure lowercase English characters via ASCII values
    else if ((letter < 97 || letter > 122) && letter != '\n') {
      printf("Invalid input - please enter lowercase English letters only!\n");
      char_count = 0;
      clear_stdin(letter);
    }
    else {
      guess[char_count] = letter;
      char_count++;
    }
    
  }
  guess[5] = '\0';
  // print useful message to user showing them their input and turn
  printf("Guess %d: %s\n", turn, guess);
}

/* print_arr
PreConditions:
*arr - character array storing data to be printed
len - num of elements to be printed

PostConditions:
prints first <len> characters in arr
*/
void printarr(const char * arr, int len) {
  for (int i = 0; i < len; i++) {
    //prints character
    printf("%c", arr[i]);
  }
  printf("\n");
}

/* index_of
PreConditions:
letter - character to be found
*list - list of characters to be searched
len - length of list

PostConditions:
Returns index of letter in array or -1 if not found in array
*/
int index_of(char letter, char *list, int len) {
  for (int i = 0; i < len; i++) {
    if (list[i] == letter) {
      // if found return index
      return i;
    }
  }
  // if not return -1
  return -1;
}

/* check_helper
PreConditions:
*unused - letters in player input that did not match
*unmatched - letters in target word that did not match
*result - current results with only 'g' and 'x'

PostConditions:
goes through unused characters and checks if they match with an unmatched letter. Updates result to 'y' and removes it from unmatched if it exists
*/
void check_helper(char *unused, char *unmatched, char *result) {
  for (int j = 0; j < LEN; j++) {
    // check if unused character is unmatched
    int pos = index_of(unused[j], unmatched, LEN);
    if (pos  > -1) {
      // if unmatched, update result and remove char from unmatched
      unmatched[pos] = '0';
      result[j] = 'y';
    }
  }
}


/* check_guess
PreConditions:
*guess - player input
*target - word to be found
*result - holds which places match between guess and target according to Wordle rules

PostConditions:
Fills result with 'g', 'y', 'x' according to Worlde rules and returns how many letters in guess matched exactly with target
*/
int check_guess(const char *guess, const char *target, char result[]) {
  char unused[LEN] = {'1'};
  char unmatched[LEN]= {'0'};
  int correct = 0;
  // compares each letter in guess to corresponding letter in target
  // updates result to 'g' if they match and 'x' otherwise
  for (int i = 0; i < LEN; i++) {
    unmatched[i] = target[i];

    if (guess[i] == target[i]) {
      correct++;
      unmatched[i] = '0';
      result[i] = 'g';
    }
    else {
      result[i] = 'x';
      unused[i] = guess[i];
    }
  }
  // call check_helper to adjust result for all letters in guess that were
  // present in target but in different positions
  check_helper(unused, unmatched, result);
  return correct;
}

/* print_report
PreConditions:
*guess - player input
result[] - array holding 'x'/'y'/'g' after running check_guess function

PostConditions:
prints each letter in guess with a green, yellow, or clear background depending on its matching index in result
*/
void print_report(const char *guess, const char result[]) {
  for (int i = 0; i < LEN; i++) {
    if (result[i] == 'g') {
      printf("\033[1;37;42m%c\033[0m", guess[i]);
    }
    else if (result[i] == 'y') {
      printf("\033[1;37;43m%c\033[0m", guess[i]);
    }
    else {
      printf("%c", guess[i]);
    } 
  }
  printf("\n");
}

/* summary_report
PreConditions:
*guess_dict - dictionary of past results

PostConditions:
prints a summary report for all attempts made
*/
void summary_report(dict *guess_dict) {
  printf("View below your summary report!\n");
  int num = guess_dict->size;
  for (int i = 0; i < num; i++) {
    print_report(guess_dict->words[i], guess_dict->words[i]);
  }
  // free the dictionary after use
  free_dict(guess_dict);
}
/* play_game
PreConditions:
*target - word to be found

PostConditions:
plays Wordle game on command line
*/
void play_game(const char *target) {
  int attempts = 0;
  char attempt[LEN+1];
  char result[LEN];
  int correct = 0;
  char *result_copy;
  dict *guesses = make_dict();
  // give 6 tries
  while (attempts < 6) {
    // get user guess, see how correct it is
    get_guess(attempts+1, attempt);
    correct = check_guess(attempt, target, result);
    // add a copy of this turns result to dict for summary report
    result_copy = make_copy(result, LEN);
    dict_add(guesses, result_copy);
    // print the result report
    print_report(attempt, result);
    attempts++;
    // correct target found
    if (correct == 5){
      // summary report
      summary_report(guesses);
      printf("You correctly deduced '%s' after %d tries!\n", target, attempts);
      return;
    }
  }
  // target not found
  printf("You failed to deduce '%s' after %d tries! :(\n", target, attempts);
  // summary report
  summary_report(guesses);
}

int main (int argc, char* argv[]) {
  // boiler plate code for File I/O taken from past lab/reading
  if (argc != 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * stream = fopen(argv[1], "r");
  if (stream == NULL) {
    fprintf(stderr, "%s: Cannot open %s: %s\n", argv[0], argv[1], strerror(errno));
    return EXIT_FAILURE;
  }

  // implicitly define dictionary
  dict * myDict = make_dict();
  // fill dictionary with words from input file
  fill_dict(myDict, stream);
  // hard coded dictionary
  //char *dicty [] = {"hello", "while", "grave", "slope", "shine", "flank"};
  // randomizing rand()
  srand(time(NULL));
  //char *targ = dicty[rand()%6];
  // choose random element from myDict
  char *targ = myDict->words[rand()%DICT_LEN];
  // play Wordle
  play_game(targ);
  // free memory after use
  free_dict(myDict);
  return 0;
}

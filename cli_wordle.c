#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <errno.h>

// chose 5 since wordle words are 5 letters
#define LEN 5
// words5 txt file has 12971 words
#define DICT_LEN 12972

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
  // printf("Guess %d: %s\n", turn, guess);
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
void print_report(const int turn, const char *guess, const char result[]) {
  printf("Guess %d: ", turn+1);
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
void summary_report(int turns, dict *guess_dict) {
  printf("View below your summary report!\n");
  int num = guess_dict->size;
  for (int i = 0; i < num; i++) {
    print_report(i, guess_dict->words[i], guess_dict->words[i]);
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
  char input[LEN+1];
  char result[LEN];
  int correct = 0;
  char *result_copy;
  dict *guesses = make_dict();
  // give 6 tries
  while (attempts < 6) {
    // get user guess, see how correct it is
    get_guess(attempts+1, input);
    correct = check_guess(input, target, result);
    // add a copy of this turns result to dict for summary report
    result_copy = make_copy(result, LEN);
    dict_add(guesses, result_copy);
    // print the result report
    print_report(attempts, input, result);
    attempts++;
    // correct target found
    if (correct == 5){
      // summary report
      summary_report(attempts, guesses);
      printf("You correctly deduced '%s' after %d tries!\n", target, attempts);
      return;
    }
  }
  // target not found
  printf("You failed to deduce '%s' after %d tries! :(\n", target, attempts);
  // summary report
  summary_report(attempts, guesses);
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

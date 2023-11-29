# Wordle Game

This is a simple implementation of the popular game Wordle in C. The game prompts the player to guess a 5-letter word, providing feedback on each attempt based on correct letters and their positions.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [How to Play](#how-to-play)
- [File Input](#file-input)
- [Game Logic](#game-logic)
- [Summary Report](#summary-report)
- [Compilation](#compilation)
- [Usage](#usage)

## Prerequisites

Make sure you have a C compiler installed on your system.

## Installation

Clone the repository to your local machine:

```bash
git clone https://github.com/your-username/wordle-game.git
```

## How to Play

1. Compile the code.
2. Run the executable, providing a file containing a list of words as a command-line argument.

## File Input

The program expects a file containing a list of words as input. The words should be separated by newline characters. An example file is provided in the repository (`words5.txt`).

## Game Logic

- The target word is randomly chosen from the input word list.
- The player has 6 attempts to guess the word.
- After each guess, the program provides feedback using 'g' (correct letter and position), 'y' (correct letter but wrong position), and 'x' (incorrect letter).
- The game ends when the correct word is guessed or after 6 attempts.

## Summary Report

After the game ends, a summary report is displayed, showing each guess and the corresponding feedback.

## Compilation

Code assumes all files are in the same directory so "dict.h", "dict.c" and the wordle file should be in the same place. Compile the code using a C compiler. Make sure to include the dict.c file as well For example:

```bash
clang -o wordle cli_wordle.c dict.c
```

## Usage

Run the compiled executable, providing the word list file as a command-line argument:

```bash
./wordle words5.txt
```

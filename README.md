# Electronic Shopping Process – A Terminal-Based Shopping List Manager

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Unix%2Flike-lightgrey)
![Status](https://img.shields.io/badge/Status-Stable-brightgreen)

**Electronic Shopping Process** is a solo-developed terminal application written in C that lets users build and manage a custom shopping list interactively. Designed to reinforce heap memory allocation and robust error handling, the program features full user input control, sorting, and clean memory management — all from the terminal.

## Features

- **Dynamic List Creation**  
  Build a list of any length. No limit on how many items or characters you enter.

- **Interactive Input with Validation**  
  Prompts guide the user through adding item names and their quantities. Handles all invalid or empty input gracefully.

- **Sorting Logic**  
  List items are sorted alphabetically, and if identical names appear, sorted further by quantity.

- **Memory Management**  
  Every element — list name, items, quantities — is dynamically allocated and properly freed after use.

- **Commands for Flow Control**  
  - `done`: Ends input and prints the final list.  
  - `quit`: Immediately exits the program without output.

---

## Getting Started

### Requirements
- C Compiler (e.g. `gcc`)
- Unix-like Terminal (Linux/macOS)

---

## Compilation

```bash
gcc -Wall -Wextra -o esp a3.c
```

---

## Usage

Run the program:

```bash
./esp
```

### Flow:

1. The program starts with a welcome message.

2. Prompts the user to enter the name of the shopping list.

3. Asks for item name and quantity, in a loop.

4. Use `done` to print the sorted list and exit.

5. Use `quit` at any time to abort immediately without output.

6. All memory is cleaned up before exit.

---

## Sample Gameplay

```
Welcome to the
Electronic Shopping Process!

What do you want to name your shopping list?
 > My Weekly Shopping

Item 1:
What do you want to add?
 > Apples
How many pieces/grams/milliliters of this item do you want to get?
 > 3

Item 2:
What do you want to add?
 > Bread
How many pieces/grams/milliliters of this item do you want to get?
 > 2

Item 3:
What do you want to add?
 > done

Shopping list: My Weekly Shopping
 * 3 Apples
 * 2 Bread
```

---

## Error Handling

Robust error handling is a key part of the Electronic Shopping Process. The program performs extensive input validation at every step to ensure a smooth and crash-free user experience:

- **Empty Input**  
  If the user presses Enter without typing anything when prompted for the list name, item name, or quantity, the program notifies the user and re-prompts for correct input.

- **Whitespace Trimming**  
  Inputs are trimmed of leading and trailing whitespace to avoid issues with blank strings or accidental spaces.

- **Invalid Quantity**  
  Quantities must be positive integers. If the user enters a non-numeric value, a negative number, or zero, the input is rejected with an error message, and the user is prompted again.

- **Command Recognition**  
  The program correctly distinguishes between valid commands (`done`, `quit`) and regular item names. Even if a user types commands in different cases (e.g., `Done`, `QUIT`), they are recognized.

- **Memory Allocation Checks**  
  Every `malloc` and `realloc` call is followed by a check to ensure memory was successfully allocated. If not, the program prints an error message and exits cleanly.

- **Graceful Exit**  
  Upon entering `quit`, the program immediately frees all allocated memory and exits. If `done` is used, the final list is printed before cleanup and exit.

---

## File Structure

```
.
├── a3.c              # Complete source code
└── README.md         # This file
```

---

## License

This project is open-source and licensed under the MIT License.

---

Solo project, written with care to demonstrate clean C programming and memory safety.

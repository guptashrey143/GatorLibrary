# Gator Library

This project is a command-line application for managing a library system. It uses a Red-Black Tree data structure to store and manage books.
The application supports various operations like inserting a book, printing a book's details, searching for the closest book, borrowing a book,
returning a book, deleting a book, and printing books within a given range.

## Files

- `gatorlibrary.cpp`: This is the main source file for the application. It contains the `main` function and the implementation of the `RedBlackTree` class and `Book` and `reservations` structs.

- `makefile.mak`: This is the makefile for the project. It specifies the compiler, compiler flags, and target, and provides targets for building, running, and cleaning the project.

## Building and Running

To build the project, use the `make` command in the terminal:

```sh
make -f makefile.mak

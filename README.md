# Library Management Project

A simple console-based C++ library management application. It lets users add
books and members, and enter book issue and return details.

## Code structure

```text
include/
  library.h++   Library class declaration
  member.h++    Member class declaration
src/
  main.c++      Menu and program entry point
  library.c++   Add Book and Add member operations
  member.c++    Book issue and return operations
```

## Libraries

This project uses the C++ Standard Library, including `<iostream>`,
`<string>`, and `<limits>`. It has no third-party library dependencies.

## Build and run

From the project root:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.c++ src/library.c++ src/member.c++ -o library
./library
```

On macOS, you can use `clang++` instead of `g++`. The compiled `library`
executable is generated locally.

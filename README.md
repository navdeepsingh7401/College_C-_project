# Library Management Project

A small C++ library management project.

## Code structure

```text
include/
  library.h++   Library class declaration and data members
src/
  main.c++      Program entry point
  library.c++   Library member function implementations
```

## Build and run

From the project root:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.c++ src/library.c++ -o main
./main
```

The compiled `main` executable is generated locally and ignored by Git.

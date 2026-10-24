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
database/
  schema.sql                       MySQL database and table definitions
  add_registration_timestamps.sql  Migration for an existing database
```

## Libraries

The project uses the C++ Standard Library and includes MySQL Connector/C++'s
X DevAPI header. Database connectivity is not implemented in the application
yet. Install the connector before compiling, and adjust its include path for
your operating system if needed. VS Code's global
`C_Cpp.default.includePath` setting should include the connector's `include`
directory.

## MySQL schema

The schema creates a `library_management` database with tables for books,
members, and book issue/return records. Run it with the MySQL command-line client:

```bash
mysql -u your_username -p < database/schema.sql
```

If you already created the database using the earlier schema, run this
migration once to add `registered_at` timestamps to the books and members:

```bash
mysql -u your_username -p < database/add_registration_timestamps.sql
```

Existing rows receive the time the migration is applied as their initial
registration timestamp. The current C++ application does not connect to MySQL;
the SQL files define and update the database structure.

## Build and run

From the project root:

```bash
clang++ -std=c++17 -Wall -Wextra -Iinclude \
  -I/opt/homebrew/opt/mysql-connector-c++/include \
  src/main.c++ src/library.c++ src/member.c++ \
  -L/opt/homebrew/opt/mysql-connector-c++/lib \
  -Wl,-rpath,/opt/homebrew/opt/mysql-connector-c++/lib \
  -lmysqlcppconnx -o library
./library
```

These connector paths are for Homebrew on Apple Silicon; adjust them for your
installation. The compiled `library` executable is generated locally.

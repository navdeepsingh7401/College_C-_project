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
  schema.sql     Complete MySQL database and table definitions
```

## Libraries

The application uses the C++ Standard Library and MySQL Connector/C++'s X
DevAPI. Install the connector before compiling. The application connects to a
MySQL server through its X Protocol endpoint.

## MySQL schema

Run the single setup file `database/schema.sql` in MySQL Workbench. It creates
the `library_management` database and all four tables used by the project:
`books`, `members`, `book_issues`, and `returned_books`. It can also be run
from the MySQL command line:

```bash
mysql -u your_username -p < database/schema.sql
```

MySQL automatically fills `registered_at`, `issue_date`, and `return_date`
using `DEFAULT CURRENT_TIMESTAMP`. The C++ app inserts only the book and
member IDs for an issue or return, so it does not need to provide those dates.
The schema's triggers also decrease available copies on issue, reject issues
when no copies remain, and increase available copies on return.

`CREATE TABLE IF NOT EXISTS` leaves existing tables unchanged. For an existing
database, the script creates any missing tables, but it will not add timestamp
columns or defaults to tables that already exist.

## Database connection settings

Copy `.env.example` to `.env` and set `DB_PASSWORD` to your MySQL password.
The `.env` file is ignored by Git. Before running the application, load these
settings into the shell environment:

```bash
cp .env.example .env
# Edit .env and set DB_PASSWORD, then:
set -a
. ./.env
set +a
./library
```

The application reads `DB_USER`, `DB_PASSWORD`, `DB_HOST`, `DB_PORT`, and
`DB_NAME` from the environment. `.env` files are not loaded automatically by
C++; sourcing the file makes its values available to the program.

## Build and run

From the project root:

```bash
connector_prefix="$(brew --prefix mysql-connector-c++)"
clang++ -std=c++17 -Wall -Wextra -Iinclude \
  -I"$connector_prefix/include" \
  src/main.c++ src/library.c++ src/member.c++ \
  -L"$connector_prefix/lib" \
  -Wl,-rpath,"$connector_prefix/lib" \
  -lmysqlcppconnx -o library
./library
```

Install the connector with Homebrew first if needed:

```bash
brew install mysql-connector-c++
```

The compiled `library` executable is generated locally.

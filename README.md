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
  schema.sql                        MySQL database and table definitions
  add_issue_date_default.sql        Migration for issue timestamps
  add_registration_timestamps.sql   Migration for registration timestamps
```

## Libraries

The application uses the C++ Standard Library and MySQL Connector/C++'s X
DevAPI. Install the connector before compiling. The application connects to a
MySQL server through its X Protocol endpoint.

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
registration timestamp. The C++ application connects to the
`library_management` schema through MySQL Connector/C++'s X DevAPI.

For an existing database, run this migration once to make MySQL assign the
current date and time when a book is issued:

```bash
mysql -u your_username -p < database/add_issue_date_default.sql
```
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

# Library Management System (C++)

A menu-driven, console-based Library Management System developed in **C++17** as a college mini project.

## Project details

- **Project title:** Library Management System
- **Course requirement:** Mini Project No. 13
- **Group members:**
  - Subodh Kamble — URN: E25B001998
  - Harsh Walaswar — URN: E25B001227
- **Project coordinator:** Sudhir Kadam

## Features

1. Add, list, search and remove books.
2. Register and list students.
3. Issue books to registered students.
4. Return books and calculate overdue fines.
5. View currently issued books.
6. Persist books, students and active loans in CSV files.
7. Validate input and report errors using custom exception handling.

## Concepts demonstrated

| Requirement | Implementation |
|---|---|
| Classes and objects | `Book`, `Person`, `Student`, `Library` |
| Encapsulation | Private data members with public methods |
| Inheritance | `Student` inherits from abstract `Person` |
| Abstraction | `Person` defines the abstract `getRole()` interface |
| Polymorphism | `getRole()` is virtual and overridden by `Student` |
| STL | `vector`, `map`, `find_if`, `transform`, `algorithm` |
| Exception handling | `LibraryException`, `try`, `catch`, `throw` |
| File handling | `ifstream` and `ofstream` with CSV storage |
| Git | Version control and collaborative commits |

## Requirements

- A C++ compiler supporting C++17 (GCC 7+, Clang 5+, or a recent Visual Studio).
- Git (for version control).

## Build and run

Open a terminal in the project root.

### Linux / macOS

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o library
./library
```

### Windows (MinGW)

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o library.exe
library.exe
```

The program reads and writes files in the `data/` directory. Run the executable from the project root so that the relative data paths work.

## How to use

1. Start the program.
2. Choose an option from the menu.
3. Add books before issuing them.
4. Register a student before issuing a book to that student.
5. Use the book ID to issue or return a book.
6. Choose `0` to save and exit.

### Library rules used in this project

- A book can have only one active issue at a time.
- Only registered students can borrow books.
- The default borrowing period is 14 days.
- An overdue fine of Rs. 5 per day is calculated after the 14-day period.
- A book cannot be removed while it is issued.

## Data files

- `data/books.csv` — book ID, title, author, category, availability.
- `data/students.csv` — student ID, name, department.
- `data/loans.csv` — book ID, student ID, issue timestamp.

The CSV files are intentionally included empty in the repository. The application fills them as records are added.

## UML class diagram

See [`docs/uml.md`](docs/uml.md).

## Suggested Git workflow

```bash
git init
git add .
git commit -m "Initial commit: library management system"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/library-management-system.git
git push -u origin main
```

For group collaboration, each member should work on a separate feature branch, test changes, and merge through pull requests.

## Limitations and possible future improvements

- Add administrator login and role-based permissions.
- Add a graphical user interface.
- Use a database such as SQLite instead of CSV files.
- Add multiple copies of the same book and reservation/waiting-list support.
- Add automated unit tests.

## Academic note

Run and test the application on your own computer before submission. Be prepared to explain the classes, STL containers, file persistence, exception handling and Git workflow during your project demonstration.

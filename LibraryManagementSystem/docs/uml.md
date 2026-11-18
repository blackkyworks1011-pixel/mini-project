# UML Class Diagram

The following Mermaid class diagram can be rendered by GitHub in Markdown-compatible Mermaid viewers.

```mermaid
classDiagram
    class Person {
        <<abstract>>
        #int id
        #string name
        +getId() int
        +getName() string
        +getRole()* string
    }

    class Student {
        -string department
        +getDepartment() string
        +getRole() string
    }

    class Book {
        -int id
        -string title
        -string author
        -string category
        -bool available
        +getId() int
        +getTitle() string
        +getAuthor() string
        +getCategory() string
        +isAvailable() bool
        +setAvailable(bool) void
        +display() void
    }

    class Loan {
        +int bookId
        +int studentId
        +long long issueDate
    }

    class Library {
        -vector~Book~ books
        -map~int, Student~ students
        -vector~Loan~ loans
        +loadData() void
        +saveData() void
        +addBook() void
        +listBooks() void
        +searchBooks() void
        +registerStudent() void
        +listStudents() void
        +issueBook() void
        +returnBook() void
        +showActiveLoans() void
        +removeBook() void
    }

    class LibraryException {
        +LibraryException(message)
    }

    Person <|-- Student
    Library o-- Book
    Library o-- Student
    Library o-- Loan
    LibraryException --|> runtime_error
```

## Relationship explanation

- `Student` inherits from the abstract `Person` class.
- `Library` manages collections of `Book`, `Student`, and `Loan` objects.
- `Loan` connects a book with the student who borrowed it.
- `LibraryException` extends `std::runtime_error` to represent application-specific errors.

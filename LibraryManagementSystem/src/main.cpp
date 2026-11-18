#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// --------------------------- Custom Exceptions ---------------------------
class LibraryException : public runtime_error {
public:
    explicit LibraryException(const string& message) : runtime_error(message) {}
};

// --------------------------- Domain Classes -------------------------------
class Person {
protected:
    int id{};
    string name;

public:
    Person() = default;
    Person(int id, string name) : id(id), name(std::move(name)) {}
    virtual ~Person() = default;

    int getId() const { return id; }
    const string& getName() const { return name; }
    virtual string getRole() const = 0;
};

class Student : public Person {
    string department;

public:
    Student() = default;
    Student(int id, string name, string department)
        : Person(id, std::move(name)), department(std::move(department)) {}

    const string& getDepartment() const { return department; }
    string getRole() const override { return "Student"; }
};

class Book {
    int id{};
    string title;
    string author;
    string category;
    bool available{true};

public:
    Book() = default;
    Book(int id, string title, string author, string category, bool available = true)
        : id(id), title(std::move(title)), author(std::move(author)),
          category(std::move(category)), available(available) {}

    int getId() const { return id; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    const string& getCategory() const { return category; }
    bool isAvailable() const { return available; }
    void setAvailable(bool value) { available = value; }

    void display() const {
        cout << left << setw(8) << id << setw(30) << title.substr(0, 28)
             << setw(24) << author.substr(0, 22) << setw(18) << category.substr(0, 16)
             << (available ? "Available" : "Issued") << '\n';
    }
};

struct Loan {
    int bookId{};
    int studentId{};
    long long issueDate{}; // Unix timestamp
};

// --------------------------- Utility Functions ---------------------------
long long currentTime() {
    return static_cast<long long>(time(nullptr));
}

string cleanField(string value) {
    // Keep the simple CSV format safe and predictable.
    replace(value.begin(), value.end(), ',', ' ');
    replace(value.begin(), value.end(), '\n', ' ');
    replace(value.begin(), value.end(), '\r', ' ');
    return value;
}

vector<string> splitCsv(const string& line) {
    vector<string> fields;
    string field;
    stringstream stream(line);
    while (getline(stream, field, ',')) fields.push_back(field);
    return fields;
}

// --------------------------- Library Manager -----------------------------
class Library {
    vector<Book> books;
    map<int, Student> students;
    vector<Loan> loans;

    const string booksFile = "data/books.csv";
    const string studentsFile = "data/students.csv";
    const string loansFile = "data/loans.csv";

    Book* findBook(int bookId) {
        auto it = find_if(books.begin(), books.end(),
                          [bookId](const Book& book) { return book.getId() == bookId; });
        return it == books.end() ? nullptr : &(*it);
    }

    const Book* findBook(int bookId) const {
        auto it = find_if(books.begin(), books.end(),
                          [bookId](const Book& book) { return book.getId() == bookId; });
        return it == books.end() ? nullptr : &(*it);
    }

    void ensureDataDirectory() const {
        // The repository includes data/. This check gives a helpful message if it is removed.
        ifstream test(booksFile);
        (void)test;
    }

public:
    Library() { loadData(); }

    void loadData() {
        books.clear();
        students.clear();
        loans.clear();

        ifstream bookInput(booksFile);
        string line;
        while (getline(bookInput, line)) {
            auto f = splitCsv(line);
            if (f.size() == 5) {
                try {
                    books.emplace_back(stoi(f[0]), f[1], f[2], f[3], f[4] == "1");
                } catch (...) {
                    cerr << "Skipping an invalid book record.\n";
                }
            }
        }

        ifstream studentInput(studentsFile);
        while (getline(studentInput, line)) {
            auto f = splitCsv(line);
            if (f.size() == 3) {
                try {
                    int studentId = stoi(f[0]);
                    students.emplace(studentId, Student(studentId, f[1], f[2]));
                } catch (...) {
                    cerr << "Skipping an invalid student record.\n";
                }
            }
        }

        ifstream loanInput(loansFile);
        while (getline(loanInput, line)) {
            auto f = splitCsv(line);
            if (f.size() == 3) {
                try {
                    loans.push_back({stoi(f[0]), stoi(f[1]), stoll(f[2])});
                } catch (...) {
                    cerr << "Skipping an invalid loan record.\n";
                }
            }
        }
    }

    void saveData() const {
        ofstream bookOutput(booksFile);
        ofstream studentOutput(studentsFile);
        ofstream loanOutput(loansFile);

        if (!bookOutput || !studentOutput || !loanOutput)
            throw LibraryException("Unable to save data. Make sure the data/ folder exists and is writable.");

        for (const auto& b : books)
            bookOutput << b.getId() << ',' << cleanField(b.getTitle()) << ','
                       << cleanField(b.getAuthor()) << ',' << cleanField(b.getCategory())
                       << ',' << (b.isAvailable() ? 1 : 0) << '\n';

        for (const auto& entry : students) {
            const auto& s = entry.second;
            studentOutput << s.getId() << ',' << cleanField(s.getName()) << ','
                          << cleanField(s.getDepartment()) << '\n';
        }

        for (const auto& loan : loans)
            loanOutput << loan.bookId << ',' << loan.studentId << ',' << loan.issueDate << '\n';
    }

    void addBook() {
        int id;
        string title, author, category;
        cout << "Book ID: "; cin >> id;
        if (findBook(id)) throw LibraryException("A book with this ID already exists.");
        cin.ignore();
        cout << "Title: "; getline(cin, title);
        cout << "Author: "; getline(cin, author);
        cout << "Category: "; getline(cin, category);
        if (title.empty() || author.empty()) throw LibraryException("Title and author cannot be empty.");
        books.emplace_back(id, cleanField(title), cleanField(author), cleanField(category));
        saveData();
        cout << "Book added successfully.\n";
    }

    void listBooks() const {
        if (books.empty()) { cout << "No books found.\n"; return; }
        cout << left << setw(8) << "ID" << setw(30) << "TITLE" << setw(24) << "AUTHOR"
             << setw(18) << "CATEGORY" << "STATUS\n";
        cout << string(90, '-') << '\n';
        for (const auto& book : books) book.display();
    }

    void searchBooks() const {
        cin.ignore();
        string keyword;
        cout << "Enter title or author keyword: ";
        getline(cin, keyword);
        transform(keyword.begin(), keyword.end(), keyword.begin(),
                  [](unsigned char c) { return static_cast<char>(tolower(c)); });

        bool found = false;
        for (const auto& book : books) {
            string searchable = book.getTitle() + " " + book.getAuthor();
            transform(searchable.begin(), searchable.end(), searchable.begin(),
                      [](unsigned char c) { return static_cast<char>(tolower(c)); });
            if (searchable.find(keyword) != string::npos) {
                if (!found) {
                    cout << left << setw(8) << "ID" << setw(30) << "TITLE"
                         << setw(24) << "AUTHOR" << setw(18) << "CATEGORY" << "STATUS\n";
                }
                book.display();
                found = true;
            }
        }
        if (!found) cout << "No matching books found.\n";
    }

    void registerStudent() {
        int id;
        string name, department;
        cout << "Student ID: "; cin >> id;
        if (students.count(id)) throw LibraryException("A student with this ID already exists.");
        cin.ignore();
        cout << "Student name: "; getline(cin, name);
        cout << "Department: "; getline(cin, department);
        if (name.empty()) throw LibraryException("Student name cannot be empty.");
        students.emplace(id, Student(id, cleanField(name), cleanField(department)));
        saveData();
        cout << "Student registered successfully.\n";
    }

    void listStudents() const {
        if (students.empty()) { cout << "No students registered.\n"; return; }
        cout << left << setw(14) << "STUDENT ID" << setw(30) << "NAME" << "DEPARTMENT\n";
        for (const auto& entry : students) {
            const auto& s = entry.second;
            cout << left << setw(14) << s.getId() << setw(30) << s.getName()
                 << s.getDepartment() << '\n';
        }
    }

    void issueBook() {
        int bookId, studentId;
        cout << "Book ID: "; cin >> bookId;
        cout << "Student ID: "; cin >> studentId;

        Book* book = findBook(bookId);
        if (!book) throw LibraryException("Book ID not found.");
        if (!students.count(studentId)) throw LibraryException("Student ID not found.");
        if (!book->isAvailable()) throw LibraryException("This book is already issued.");

        book->setAvailable(false);
        loans.push_back({bookId, studentId, currentTime()});
        saveData();
        cout << "Book issued successfully to " << students.at(studentId).getName() << ".\n";
    }

    void returnBook() {
        int bookId;
        cout << "Book ID to return: "; cin >> bookId;
        auto it = find_if(loans.begin(), loans.end(),
                          [bookId](const Loan& loan) { return loan.bookId == bookId; });
        if (it == loans.end()) throw LibraryException("No active issue record found for this book.");

        const long long secondsPerDay = 24LL * 60 * 60;
        long long days = (currentTime() - it->issueDate) / secondsPerDay;
        long long overdueDays = max(0LL, days - 14);
        const int finePerDay = 5;

        Book* book = findBook(bookId);
        if (book) book->setAvailable(true);
        loans.erase(it);
        saveData();

        cout << "Book returned successfully.\n";
        cout << "Days kept: " << days << "\n";
        cout << "Overdue fine (Rs. " << finePerDay << "/day after 14 days): Rs. "
             << overdueDays * finePerDay << "\n";
    }

    void showActiveLoans() const {
        if (loans.empty()) { cout << "There are no active loans.\n"; return; }
        cout << left << setw(12) << "BOOK ID" << setw(14) << "STUDENT ID"
             << setw(28) << "STUDENT NAME" << "ISSUE DATE\n";
        for (const auto& loan : loans) {
            auto student = students.find(loan.studentId);
            time_t raw = static_cast<time_t>(loan.issueDate);
            tm* date = localtime(&raw);
            cout << left << setw(12) << loan.bookId << setw(14) << loan.studentId
                 << setw(28) << (student != students.end() ? student->second.getName() : "Unknown");
            if (date) cout << put_time(date, "%d-%m-%Y");
            cout << '\n';
        }
    }

    void removeBook() {
        int id;
        cout << "Book ID to remove: "; cin >> id;
        auto it = find_if(books.begin(), books.end(),
                          [id](const Book& book) { return book.getId() == id; });
        if (it == books.end()) throw LibraryException("Book ID not found.");
        if (!it->isAvailable()) throw LibraryException("Return the book before deleting it.");
        books.erase(it);
        saveData();
        cout << "Book removed successfully.\n";
    }
};

// --------------------------- Main Application -----------------------------
void showMenu() {
    cout << "\n========== LIBRARY MANAGEMENT SYSTEM ==========\n"
         << "1. Add book\n"
         << "2. View all books\n"
         << "3. Search books\n"
         << "4. Register student\n"
         << "5. View students\n"
         << "6. Issue book\n"
         << "7. Return book\n"
         << "8. View active loans\n"
         << "9. Remove book\n"
         << "0. Save and exit\n"
         << "Choose an option: ";
}

int main() {
    try {
        Library library;
        cout << "Welcome to the Library Management System!\n";
        bool running = true;

        while (running) {
            showMenu();
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }

            try {
                switch (choice) {
                    case 1: library.addBook(); break;
                    case 2: library.listBooks(); break;
                    case 3: library.searchBooks(); break;
                    case 4: library.registerStudent(); break;
                    case 5: library.listStudents(); break;
                    case 6: library.issueBook(); break;
                    case 7: library.returnBook(); break;
                    case 8: library.showActiveLoans(); break;
                    case 9: library.removeBook(); break;
                    case 0:
                        library.saveData();
                        cout << "Data saved. Goodbye!\n";
                        running = false;
                        break;
                    default: cout << "Invalid option. Choose between 0 and 9.\n";
                }
            } catch (const LibraryException& ex) {
                cerr << "Library error: " << ex.what() << '\n';
            } catch (const exception& ex) {
                cerr << "Unexpected error: " << ex.what() << '\n';
            }
        }
    } catch (const exception& ex) {
        cerr << "Application could not start: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}

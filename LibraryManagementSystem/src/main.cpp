#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Book {
public:
    int id;
    string title;
    bool available = true;
};

class Student {
public:
    int id;
    string name;
};

vector<Book> books;
vector<Student> students;

void addBook() {
    Book b;

    cout << "Book ID: ";
    cin >> b.id;

    cin.ignore();
    cout << "Book Title: ";
    getline(cin, b.title);

    books.push_back(b);
    cout << "Book Added Successfully!\n";
}

void viewBooks() {
    if (books.empty()) {
        cout << "No Books Available.\n";
        return;
    }

    for (auto &b : books) {
        cout << "ID: " << b.id
             << " | Title: " << b.title
             << " | Status: "
             << (b.available ? "Available" : "Issued")
             << endl;
    }
}

void registerStudent() {
    Student s;

    cout << "Student ID: ";
    cin >> s.id;

    cin.ignore();
    cout << "Student Name: ";
    getline(cin, s.name);

    students.push_back(s);
    cout << "Student Registered!\n";
}

void issueBook() {
    int bookId;

    cout << "Book ID: ";
    cin >> bookId;

    for (auto &b : books) {
        if (b.id == bookId) {
            if (b.available) {
                b.available = false;
                cout << "Book Issued Successfully!\n";
            } else {
                cout << "Book Already Issued!\n";
            }
            return;
        }
    }

    cout << "Book Not Found!\n";
}

void returnBook() {
    int bookId;

    cout << "Book ID: ";
    cin >> bookId;

    for (auto &b : books) {
        if (b.id == bookId) {
            b.available = true;
            cout << "Book Returned Successfully!\n";
            return;
        }
    }

    cout << "Book Not Found!\n";
}

int main() {
    int choice;

    do {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. View Books\n";
        cout << "3. Register Student\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "0. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: registerStudent(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid Choice!\n";
        
        }

    } while (choice != 0);

    return 0;
}

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  // For std::find_if, etc.
using namespace std;

class Date {
private:
    int dd, mm, yy;

    void rectifyDate() {
        if (dd > 30) {
            dd -= 30;
            mm++;
        }
        if (mm > 12) {
            mm -= 12;
            yy++;
        }
    }

public:
    Date() : dd(21), mm(4), yy(2023) {}
    Date(int d, int m, int y) : dd(d), mm(m), yy(y) {}

    void setDate() {
        cout << "Enter Day: "; cin >> dd;
        cout << "Enter Month: "; cin >> mm;
        cout << "Enter Year: "; cin >> yy;
    }

    void setDate(Date temp) {
        dd = temp.dd + 15;
        mm = temp.mm;
        yy = temp.yy;
        rectifyDate();
    }

    void showDate() const {
        cout << dd << "/" << mm << "/" << yy;
    }
};

class Book {
public:
    string title;
    string author;
    string publisher;
    int status;         // 0: available, 1: issued
    float price;
    int issuedRollNo;
    Date issueDate, returnDate;

    Book() : status(0), issuedRollNo(-1) {}

    void inputDetails() {
        cout << "Enter book title: "; cin.ignore(); getline(cin, title);
        cout << "Enter author's name: "; getline(cin, author);
        cout << "Enter publisher: "; getline(cin, publisher);
        cout << "Enter price: "; cin >> price;
    }

    void showDetails() const {
        cout << "Title: " << title << "\nAuthor: " << author << "\nPublisher: " << publisher << "\nPrice: " << price << endl;
        if (status == 1) {
            cout << "Issued to Roll No: " << issuedRollNo << "\nIssue Date: "; issueDate.showDate();
            cout << "\nReturn Date: "; returnDate.showDate();
        } else {
            cout << "Available\n";
        }
    }
};

class Student {
public:
    int rollNo;
    string name, address, branch;
    int status;         // 0: no book issued, 1: book issued
    string bookTitle;

    Student() : status(0), rollNo(-1) {}

    void inputDetails() {
        cout << "Enter student's roll no: "; cin >> rollNo;
        cin.ignore();
        cout << "Enter student's name: "; getline(cin, name);
        cout << "Enter address: "; getline(cin, address);
        cout << "Enter branch: "; getline(cin, branch);
    }

    void showDetails() const {
        cout << "Roll No: " << rollNo << "\nName: " << name << "\nAddress: " << address << "\nBranch: " << branch << endl;
        if (status == 1) {
            cout << "Issued Book: " << bookTitle << endl;
        } else {
            cout << "No book issued." << endl;
        }
    }
};

class Library {
private:
    vector<Student> students;
    vector<Book> books;

public:
    void addStudent() {
        Student s;
        s.inputDetails();
        students.push_back(s);
        cout << "Student added successfully!\n";
    }

    void addBook() {
        Book b;
        b.inputDetails();
        books.push_back(b);
        cout << "Book added successfully!\n";
    }

    void issueBook() {
        int rollNo;
        string title;

        cout << "Enter student's roll no: "; cin >> rollNo;
        cout << "Enter book title: "; cin.ignore(); getline(cin, title);

        auto student = find_if(students.begin(), students.end(), [rollNo](Student &s) { return s.rollNo == rollNo; });
        auto book = find_if(books.begin(), books.end(), [title](Book &b) { return b.title == title; });

        if (student != students.end() && book != books.end()) {
            if (student->status == 0 && book->status == 0) {
                student->status = 1;
                student->bookTitle = book->title;
                book->status = 1;
                book->issuedRollNo = student->rollNo;
                book->issueDate.setDate();
                book->returnDate.setDate(book->issueDate);
                cout << "Book issued successfully!\n";
            } else {
                cout << "Either the student has already issued a book or the book is unavailable.\n";
            }
        } else {
            cout << "Student or book not found.\n";
        }
    }

    void returnBook() {
        int rollNo;
        cout << "Enter student's roll no: "; cin >> rollNo;

        auto student = find_if(students.begin(), students.end(), [rollNo](Student &s) { return s.rollNo == rollNo; });
        if (student != students.end() && student->status == 1) {
            auto book = find_if(books.begin(), books.end(), [&student](Book &b) { return b.title == student->bookTitle; });

            if (book != books.end()) {
                student->status = 0;
                book->status = 0;
                student->bookTitle = "";
                book->issuedRollNo = -1;
                cout << "Book returned successfully!\n";
            } else {
                cout << "Book not found in the system!\n";
            }
        } else {
            cout << "Student has no issued book or not found!\n";
        }
    }

    void viewAllStudents() {
        for (const auto &s : students) {
            s.showDetails();
            cout << "--------------------\n";
        }
    }

    void viewAllBooks() {
        for (const auto &b : books) {
            b.showDetails();
            cout << "--------------------\n";
        }
    }
};

int main() {
    Library library;
    int choice;
    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Student\n2. Add Book\n3. Issue Book\n4. Return Book\n5. View All Students\n6. View All Books\n7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                library.addStudent();
                break;
            case 2:
                library.addBook();
                break;
            case 3:
                library.issueBook();
                break;
            case 4:
                library.returnBook();
                break;
            case 5:
                library.viewAllStudents();
                break;
            case 6:
                library.viewAllBooks();
                break;
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 7);

    return 0;
}

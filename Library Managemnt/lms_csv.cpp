#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>      // For file operations
#include <sstream>     // For string stream
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
    Date() : dd(20), mm(10), yy(2024) {}
    Date(int d, int m, int y) : dd(d), mm(m), yy(y) {}

    void setDate() {
        cout << "Enter Day: "; cin >> dd;
        cout << "Enter Month: "; cin >> mm;
        cout << "Enter Year: "; cin >> yy;
        rectifyDate();  // Ensure the date is valid
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

    string getFormattedDate() const {  // New method to get formatted date
        return to_string(dd) + "/" + to_string(mm) + "/" + to_string(yy);
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

    void saveStudentsToCSV(const vector<Student>& students) {
        ofstream file("students.csv");
        if (file.is_open()) {
            for (const auto& student : students) {
                file << student.rollNo << ","
                     << student.name << ","
                     << student.address << ","
                     << student.branch << ","
                     << student.status << ","
                     << student.bookTitle << "\n";
            }
            file.close();
        } else {
            cout << "Error opening file to save student data.\n";
        }
    }

    void loadStudentsFromCSV() {
        ifstream file("students.csv");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                Student student;
                stringstream ss(line);
                string tempStatus;

                getline(ss, line, ','); student.rollNo = stoi(line);
                getline(ss, student.name, ',');
                getline(ss, student.address, ',');
                getline(ss, student.branch, ',');
                getline(ss, tempStatus, ','); student.status = stoi(tempStatus);
                getline(ss, student.bookTitle, ',');

                students.push_back(student);
            }
            file.close();
        } else {
            cout << "No student data found. Starting fresh.\n";
        }
    }

    void saveBooksToCSV(const vector<Book>& books) {
        ofstream file("books.csv");
        if (file.is_open()) {
            for (const auto& book : books) {
                file << book.title << ","
                     << book.author << ","
                     << book.publisher << ","
                     << book.price << ","
                     << book.status << ","
                     << book.issuedRollNo << ","
                     << book.issueDate.getFormattedDate() << ","
                     << book.returnDate.getFormattedDate() << "\n";  // Use the new method
            }
            file.close();
        } else {
            cout << "Error opening file to save book data.\n";
        }
    }

    void loadBooksFromCSV() {
        ifstream file("books.csv");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                Book book;
                stringstream ss(line);
                string tempStatus, tempIssuedRollNo;

                getline(ss, book.title, ',');
                getline(ss, book.author, ',');
                getline(ss, book.publisher, ',');
                ss >> book.price; ss.ignore(1, ',');
                getline(ss, tempStatus, ','); book.status = stoi(tempStatus);
                getline(ss, tempIssuedRollNo, ','); book.issuedRollNo = stoi(tempIssuedRollNo);
                // Set issueDate and returnDate if needed (parsing is not implemented here)

                books.push_back(book);
            }
            file.close();
        } else {
            cout << "No book data found. Starting fresh.\n";
        }
    }

public:
    Library() {
        loadStudentsFromCSV();  // Load data when library object is created
        loadBooksFromCSV();
    }

    void addStudent() {
        Student s;
        s.inputDetails();
        students.push_back(s);
        saveStudentsToCSV(students);  // Save to CSV after adding student
        cout << "Student added successfully and data saved to CSV!\n";
    }

    void addBook() {
        Book b;
        b.inputDetails();
        books.push_back(b);
        saveBooksToCSV(books);  // Save to CSV after adding book
        cout << "Book added successfully and data saved to CSV!\n";
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

                saveStudentsToCSV(students);  // Update CSV after issuing book
                saveBooksToCSV(books);
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

                saveStudentsToCSV(students);  // Update CSV after returning book
                saveBooksToCSV(books);
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
            cout << "\n--------------------\n";
        }
    }

    void viewAllBooks() {
        for (const auto &b : books) {
            b.showDetails();
            cout << "\n--------------------\n";
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

#include <iostream>
using namespace std;

class Student {
protected:
    int rollno;
    string name;

public:
    void getdata() {
        cout << "\nEnter Roll Number: ";
        cin >> rollno;
        cout << "Enter Name: ";
        cin >> name;
    }

    void putdata() {
        cout << "\nRoll Number: " << rollno;
        cout << "\nName: " << name;
    }
};

class StudentExam : public Student {
protected:
    float sub1, sub2, sub3, sub4, sub5, sub6;

public:
    void accept_data() {
        getdata();
        cout << "Enter Marks for 6 Subjects:\n";
        cin >> sub1 >> sub2 >> sub3 >> sub4 >> sub5 >> sub6;
    }

    void display_data() {
        putdata();
        cout << "\nMarks in 6 Subjects: "
             << sub1 << " " << sub2 << " " << sub3 << " "
             << sub4 << " " << sub5 << " " << sub6;
    }
};

class StudentResult : public StudentExam {
    float percent;

public:
    void calculate() {
        percent = (sub1 + sub2 + sub3 + sub4 + sub5 + sub6) / 6.0;
        cout << "\nTotal Percentage: " << percent << "%";
    }
};

int main() {
    StudentResult sr;
    cout << "\nEnter Student Details:";
    sr.accept_data();
    cout << "\n\n--- Student Result ---";
    sr.display_data();
    sr.calculate();

    return 0;
}

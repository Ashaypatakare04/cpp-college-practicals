#include <iostream>
#include <string>
using namespace std;

class Employee {
protected:
    int no;
    string name;

public:
    virtual void salary() = 0;
    virtual void display() = 0;
};

class SalariedEmployee : public Employee {
private:
    double monthlySalary;

public:
    void salary() {
        cout << "Enter employee number: ";
        cin >>no;
        cout << "Enter employee name: ";
        cin >> name;
        cout << "Enter employee monthly salary: ";
        cin >> monthlySalary;
    }

    void display() {
        cout << "\nSalaried Employee:\n";
        cout << "Employee Number: " << no << endl;
        cout << "Employee Name: " << name << endl;
        cout << "Salary: " << monthlySalary << endl;
    }
};

class HourlyEmployee : public Employee {
private:
    double hours;
    double rate;

public:
    void salary() {
        cout << "Enter employee number: ";
        cin >> no;
        cout << "Enter employee name: ";
        cin >> name;
        cout << "Enter hours: ";
        cin >> hours;
        cout << "Enter hourly rate: ";
        cin >> rate;
    }

    void display() {
        cout << "\nHourly Employee:\n";
        cout << "Employee Number: " << no << endl;
        cout << "Employee Name: " << name << endl;
        cout << "Hours: " << hours << endl;
        cout << "Hourly rate: " << rate << endl;
        cout << "Salary: " << (rate * hours) << endl;
    }
};

class CommissionedEmployee : public Employee {
private:
    double salarydouble;
    int overtime;

public:
    void salary() {
        cout << "Enter employee number: ";
        cin >> no;
        cout << "Enter employee name: ";
        cin >> name;
        cout << "Enter salary: ";
        cin >> salarydouble;
        cout << "Enter overtime: ";
        cin >> overtime;
    }

    void display() {
        cout << "\nCommissioned Employee:\n";
        cout << "Employee Number: " << no << endl;
        cout << "Employee Name: " << name << endl;
        cout << "Salary: " << salarydouble << endl;
        cout << "Overtime: " << overtime << endl;
        cout << "Total Salary: " << (salarydouble + overtime * 15) << endl;
    }
};

int main() {
    Employee* employees[3];

    SalariedEmployee s;
    HourlyEmployee h;
    CommissionedEmployee c;

    employees[0] = &s;
    employees[1] = &h;
    employees[2] = &c;

    for (int i = 0; i < 3; i++) {
        employees[i]->salary();
        cout << endl;
    }

    for (int i = 0; i < 3; i++) {
        employees[i]->display();
    }

    return 0;
}

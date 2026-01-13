#include <iostream>
#include <string>
using namespace std;

class user {
private:
    string name;
    long long no;
    string acc_type;
    int acc_no;

public:
    void input() {
        cout << "Enter your name: ";
        getline(cin, name); // better for full names
        cout << "Enter your mobile number: ";
        cin >> no;
        cout << "Enter your account type: ";
        cin >> acc_type;
        cout << "Enter your account number: ";
        cin >> acc_no;
        cin.ignore(); // clear input buffer
    }

    void output() {
        cout << "\n--- User Details ---\n";
        cout << "Name: " << name << endl;
        cout << "Mobile No.: " << no << endl;
        cout << "Account Type: " << acc_type << endl;
        cout << "Account No.: " << acc_no << endl;
    }
};

class bank {
private:
    float balance = 0.0;

public:
    void deposit() {
        float amount;
        cout << "Enter amount to deposit: ";
        cin >> amount;
        balance += amount;
        cout << "New Balance: " << balance << endl;
    }

    void withdraw() {
        float amount;
        cout << "Enter amount to withdraw: ";
        cin >> amount;
        if (amount <= balance) {
            balance -= amount;
            cout << "Remaining Balance: " << balance << endl;
        } else {
            cout << "Error: Withdrawal amount exceeds current balance.\n";
        }
    }

    void show_balance() {
        cout << "Current Balance: " << balance << endl;
    }
};

int main() {
    user u1;
    u1.input();
    u1.output();

    bank b1;
    b1.deposit();
    b1.withdraw();
    b1.show_balance();

    return 0;
}

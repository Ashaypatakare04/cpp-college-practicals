#include <iostream>
#include <exception>
using namespace std;

class MyException : public exception {
public:
    const char* what() const noexcept {
        return "Attempted to divide by zero!\n";
    }
};

int main() {
    try {
        int x, y;
        cout << "Enter the two numbers:\n";
        cin >> x >> y;

        if (y == 0) {
            MyException ex;
            throw ex;
        } else {
            cout << "x / y = " << (double)x / y << endl;
        }
    }
    catch (exception& e) {
        cout << e.what();
    }

    return 0;
}

#include <iostream>
using namespace std;

class Complex {
private:
    int real;        // Real part
    int imaginary;   // Imaginary part

public:
    // Default constructor
    Complex() {
        real = 0;
        imaginary = 0;
    }

    // Parameterized constructor
    Complex(int r, int i) {
        real = r;
        imaginary = i;
    }

    // Function to add two complex numbers
    Complex addComplexNumber(Complex c1, Complex c2) {
        Complex res;
        res.real = c1.real + c2.real;
        res.imaginary = c1.imaginary + c2.imaginary;
        return res;
    }

    // Function to subtract two complex numbers
    Complex subtractComplexNumber(Complex c1, Complex c2) {
        Complex res;
        res.real = c1.real - c2.real;
        res.imaginary = c1.imaginary - c2.imaginary;
        return res;
    }

    // Function to display a complex number
    void display() {
        cout << real << " + i" << imaginary << endl;
    }
};

int main() {
    Complex c1(4, 5);  // First complex number
    Complex c2(8, 9);  // Second complex number
    Complex c3;        // Result object

    cout << "First Complex Number: ";
    c1.display();
    cout << "Second Complex Number: ";
    c2.display();

    // Addition
    c3 = c3.addComplexNumber(c1, c2);
    cout << "\nSum of Complex Numbers: ";
    c3.display();

    // Subtraction
    c3 = c3.subtractComplexNumber(c1, c2);
    cout << "Difference of Complex Numbers: ";
    c3.display();

    return 0;
}

#include <iostream>
using namespace std;
class Number {
    int value;
public:
    Number(int v = 0) : value(v) {}
    void display() const {
        cout << "Value = " << value << endl;
    }   
    Number operator-() const {
        return Number(-value);
    }
    Number operator+(const Number& obj) const {
        return Number(value + obj.value);
    }
    Number operator-(const Number& obj) const {
        return Number(value - obj.value);
    }
    bool operator==(const Number& obj) const { 
        return value == obj.value; }
    bool operator!=(const Number& obj) const { 
        return value != obj.value; }
    bool operator<(const Number& obj) const { 
        return value < obj.value; }
    bool operator>(const Number& obj) const { 
        return value > obj.value; }
    bool operator<=(const Number& obj) const { 
        return value <= obj.value; }
    bool operator>=(const Number& obj) const { 
        return value >= obj.value; }
    
    friend ostream& operator<<(ostream& out, const Number& obj);
    friend istream& operator>>(istream& in, Number& obj);
};
ostream& operator<<(ostream& out, const Number& obj) {
    out << obj.value;
    return out;
}
istream& operator>>(istream& in, Number& obj) {
    in >> obj.value;
    return in;
}

int main() {
    Number n1, n2, n3;
    cout << "Enter two numbers: ";
    cin >> n1 >> n2;
    cout << "\nn1 = " << n1 << "\nn2 = " << n2 << endl;
    n3 = n1 + n2;
    cout << "\nAddition: " << n3 << endl;
    n3 = n1 - n2;
    cout << "Subtraction: " << n3 << endl;
    n3 = -n1;
    cout << "\nUnary minus applied to n1: " << n3 << endl;
    cout << "\nRelational Operations:\n";
    cout << (n1 == n2 ? "n1 == n2" : "n1 != n2") << endl;
    cout << (n1 > n2 ? "n1 > n2" : "n1 <= n2") << endl;
    cout << (n1 < n2 ? "n1 < n2" : "n1 >= n2") << endl;
    return 0;
}

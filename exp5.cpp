#include <iostream>
using namespace std;

float area(float s) {
    return (s * s); // square
}

float area(float l, float b) {
    return (l * b); // rectangle
}

float area(double r) {
    return (3.14 * r * r); // circle
}

float area(float base, float height, int) {
    return (0.5 * base * height); // triangle
}

int main() {
    float s, l, b, base, height;
    double r;

    cout << "Enter side of square: ";
    cin >> s;
    cout << "Enter length and breadth of rectangle: ";
    cin >> l >> b;
    cout << "Enter radius of circle: ";
    cin >> r;
    cout << "Enter base and height of triangle: ";
    cin >> base >> height;

    cout << "\nArea of Square: " << area(s);
    cout << "\nArea of Rectangle: " << area(l, b);
    cout << "\nArea of Circle: " << area(r);
    cout << "\nArea of Triangle: " << area(base, height, 0);

    return 0;
}

#include<iostream>
#include<cmath>
#include<string>  
const double pi = 3.14159;      
using namespace std;
class Shape {
public:
    virtual double area() const = 0;    
    virtual double perimeter() const = 0; 
};
class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override {
        return pi * radius * radius;
    }
    double perimeter() const override {
        return 2 * pi * radius;
    }
};
class Rectangle : public Shape {
    double length;
    double width;
public:
    Rectangle(double l, double w) : length(l), width(w) {}
    double area() const override {
        return length * width;
    }
    double perimeter() const override {
        return 2 * (length + width);
    }
};
class Triangle : public Shape {
private:
    double a, b, c;   
public:
    Triangle(double side1, double side2, double side3) : a(side1),
        b(side2), c(side3) {}
    double area() const override {
        double s = (a + b + c) / 2; 
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }
    double perimeter() const override {
        return a + b + c;
    }
};
int main() {
    Circle circle(5.0);
    Rectangle rectangle(4.0, 6.0);
    Triangle triangle(3.0, 4.0, 5.0);
    cout<<"Circle :";
    cout << "\nArea: " << circle.area() << endl;
    cout << "\nPerimeter: " << circle.perimeter() << endl;
    cout<<"\nRectangle :";
    cout << "\nArea: " << rectangle.area() << endl;
    cout << "Perimeter: " << rectangle.perimeter() << endl;
    cout<<"\nTriangle :";
    cout << "\nArea: " << triangle.area() << endl;
    cout << "Perimeter: " << triangle.perimeter() << endl;
    return 0;
}
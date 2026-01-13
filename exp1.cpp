#include<iostream>
using namespace std;

class Student {

private:

char name[30];

int rollNo;

float marks;

public:

void input() {

cout<<" Enter name :"<<endl;

cin>>name;

cout<<" Enter rn:"<<endl;

cin >>rollNo;

cout<<"Enter marks"<<endl;

cin>>marks;
}
void display()
{
cout<<"Name :"<<name<< endl;

cout <<"Roll No.:"<< rollNo<<endl;

cout<<"Marks:" <<marks<<endl;
}};

int main() {

Student obj1;

obj1.input();

cout <<" Details"<<endl;

obj1. display();

return 0;

}
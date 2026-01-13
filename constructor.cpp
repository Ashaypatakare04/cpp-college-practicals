#include<iostream>
using namespace std;
class rect{

private:

float length;

float width;

public:

rect() {

length = 4;

width=5;

cout<<"Default Constructor "<<endl;
}
rect (float l, float w){

length=l;

width= w;

cout <<" Parameter Constructor"<<endl;

}

rect (const rect &r) {

length=r.length;

width=r.width;

cout << "Copy constructor" << endl;
}
float area () {

return length*width;

}

float peri(){

return 2*(length+width);
}

void display(){

cout << "l: "<<length<<endl;
cout<< "W: "<< width << endl;
cout << "area: " <<area()<<endl;
cout << "perimeler: "<< peri() <<endl;
}
~rect(){

cout << "Destructor called "<< endl;

}};

int main(){

rect r1;

r1.display();
rect r2(5,7);

r2. display();

rect r3=r2;

r3. display();

return 0;

}
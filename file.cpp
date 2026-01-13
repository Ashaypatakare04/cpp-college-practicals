#include <iostream>
#include <fstream>    
#include <string>
using namespace std;
int main(){
    string s;
    int roll_no;
    ofstream f1("student.txt");
    cout<<"Enter Name:";
    getline(cin, s);
    f1<<s<<"\n";
    cout<<"Enter Roll No:";
    cin>>roll_no;
    f1<<roll_no<<"\n";
    f1.close();
    ifstream f2("student.txt");
    getline(f2, s);
    f2>>roll_no;
    cout<<"\nName: "<<s<<"\n";
    cout<<"Roll No: "<<roll_no<<"\n";
    f2.close();
    return 0;
}
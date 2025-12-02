#include <iostream>

int main ()
{
    using namespace std;

int *p_number {new int {67}}; //address 1

int number {55}; //address 2

p_number = &number; // memory leak occurs here
cout << "Value of p_number: " << *p_number << endl;// p_number points to address 2 but address 1 is still in use, our program has lost access to that memory location.

    return 0;
}
#include <iostream>
#include <math.h>

int main ()
{
    using namespace std;

    int * p_number = new int; // allocate an integer
    *p_number = 100; // assign a value to the allocated memory
    
    cout << "Value of p_number: " << *p_number << endl;
    
    delete p_number; // free the allocated memory
    p_number = nullptr; // set pointer to nullptr after deletion
    
    return 0;
}



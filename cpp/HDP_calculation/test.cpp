#include <iostream> //input, output stream
#include <string>   //string class
#include <math.h>
int main() 
{
      using namespace std;
      int entry {};
      double nhdp {};
      double rhdp {};
      double dhdp {};
        cout << "Choose calculation:" << endl;
        cout << "Výpočet HDP 1" << endl;
        cout << "Deflátor HDP 2" << endl;
        cin >> entry;

        if (entry == 1)
            {
                cout << "1" << endl;
            }
        if (entry == 2)
            {
                cout << "Enter nominální HDP" << endl;
                cin >> nhdp;
                cout << "Enter realný HDP" << endl;
                cin >> rhdp;
                dhdp = (nhdp/rhdp)*100;
                cout << "Deflátor HDP je: " << dhdp <<endl;
            }
            else 
            {
                cout << "Invalid entry" << endl;
            }

      return 0;
}

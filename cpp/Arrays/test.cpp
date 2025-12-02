#include <iostream> //input, output stream
#include <string>   //string class
#include <math.h>
int main() 
{
      using namespace std;
      /*
      string age [5] = {"29", "3", "51", "32", "63"};

      double sum_arrays = 0;
      double sum_age = 0;

      for (int i = 0; i < 5; i++) 
      {
          sum_arrays = stoi(age[i]);
          sum_age = sum_age + sum_arrays;
      }

      double avg_age = sum_age / 5;
      
      cout << "Average age is:" << avg_age;

      */

      //2D array
      int arr[3][4] =
      {
          {1, 2, 3, 4},
          {5, 6, 7, 8},
          {9, 10, 11, 12}
      };
      cout << arr[1][1]; //prints 6

      return 0;
}

#include <iostream>
#include <string>
/*
int main() {

std::string item {};
double item_price {};
double quantity {};
double total_cost {};


std::cout << "What are you buying today?" << std::endl;
std::cin >> item ; //std::getline(std::cin, variable) is better bcos if name have space it would break
std::cout << "How much does it cost per unit?" << std::endl;
std::cin >> item_price ;
std::cout << "How many are you buying?" << std::endl;
std::cin >> quantity ;

total_cost = item_price*quantity;
if (total_cost > 20){
        total_cost = total_cost * 0.9;
    }
std::cout << "----------------------------------" << std::endl;
std::cout << "RECEIPT:" << std::endl;
std::cout << "Item:" << item << std::endl;
std::cout << "Quantity:" << quantity << std::endl;
std::cout << "Total Cost:" << total_cost << std::endl;
std::cout << "----------------------------------" << std::endl;


return 0;


//Task: Ask for an integer. Check if it is Even or Odd and print the result.

int x {};

std::cout << "Input number:" << std::endl;
std::cin >> x;

if (x % 2 == 0) {
    std::cout << "Your number is even";
} else {
    std::cout << "Your number is odd";
}

return 0;


The Goal: Create a program that acts like a bouncer at a club, but also checks for senior discounts.
Requirements:
Ask the user for their age.
IF the age is less than 18:
Print: "You are too young to enter."
ELSE IF the age is greater than 65:
Print: "Welcome! You qualify for a senior discount."
ELSE (for everyone else, aged 18 to 65):
Print: "Welcome to the club."



int age {};

std::cout << "What is your age" << std::endl;
std::cin  >> age ;

if (age < 18) {
    std::cout << "You are too young to enter" << std::endl;
}
else if (age > 65){
    std::cout << "Welcome! You qualify for a senior discount" << std::endl;
}
else {
    std::cout << "Welcome to the club" << std::endl;
}


Create an integer variable secret_number and set it to whatever you want (e.g., 77).
Create an integer variable guess.
Start a Loop: The code inside must repeat forever (or until the game is won).
Inside the loop:
Ask the user "Guess the number:".
IF the guess is too high, print "Too high!".
IF the guess is too low, print "Too low!".
IF the guess is correct, print "You won!" and break the loop.

int secret_number = 32;
int guess {};

while (true) {
    std::cout << "Guess the number: ";
    std::cin >> guess;
    
    if (guess == secret_number) {
        std::cout << "You won!";
        break;
    }
    else if (guess > secret_number) {
        std::cout << "Too high!" << std::endl;
    }
    else {
        std::cout << "Too low!" << std::endl;
    }
}

The Goal: Create a program that asks the user to enter 5 different grades, stores them in an array, and then calculates the average score.
Requirements
Declare an array of integers with size 5 (e.g., int grades[5];).
Create a variable for the sum (initialize it to 0!).
Loop 1 (Input): Use a for loop to ask the user for a grade 5 times and save it into the array (using cin >> grades[i]).
Loop 2 (Calculation): Use another for loop to go through the array, read the numbers back, and add them to your sum variable.
Math: Calculate the average (Sum / 5).
Output: Print the average.

int grades [5];
int sum {};
double average;

std::cout << "Please enter 5 grades:" << std::endl;

for (int i = 0;   i < 5; i++) {
        std::cout << "Enter grade " << i + 1 << ": ";   
        std::cin >> grades[i];
}

for (int i = 0; i < 5; i++) {
        // ERROR FIX: Add the VALUE inside the slot to the sum
        sum = sum + grades[i]; 
    }
average = sum / 5.0;

    std::cout << "----------------" << std::endl;
    std::cout << "Total Sum: " << sum << std::endl;
    std::cout << "Average: " << average << std::endl;

*/

/*
// pending review (task :  Create array of 5 ints. Input & print.)

int score [5];
std::cout << "The Score Keeper" << std::endl;
std::cout << "-----------------------" << std::endl;
for (int i = 0; i < 5; i++){
    std::cout << "Enter score:" << std::endl;
    std::cin >> score [i];
}
std::cout << "-----------------------" << std::endl;
std::cout << "Scores submited:" << std::endl;
std::cout << score[0] << std::endl;
std::cout << score[1] << std::endl;
std::cout << score[2] << std::endl;
std::cout << score[3] << std::endl;
std::cout << score[4] << std::endl;



// Assume slot 0 has the biggest number
// Loop through slots 1, 2, 3, 4
// If you find a bigger number, update which slot is biggest

int array [5];
int biggest = 0;

std::cout << "Max Array Number Finder" << std::endl;
std::cout << "-----------------------" << std::endl;
for (int i = 0; i < 5; i++){
    std::cout << "Enter number:" << std::endl;
    std::cin >> array [i];
}
for (int i = 0; i < 5; i++){
    if (array[i]>array[biggest]){
        biggest = i;
         }
    // Found a bigger number!
        // Update biggest to point to this new slot
    }

std::cout << "Biggets array is:" << biggest << std::endl;





int array [5];

std::cout << "Fill the array:" << std::endl;

for ( int i=0; i < 5; i ++){
    std::cin >> array [i];
}
for ( int i = 4; i >= 0; i --){
    std::cout << array [i];
}
*/

void print_banner (){
    std::cout << "---Welcome---" << std::endl;
}

int main (){

print_banner();

return 0;
}
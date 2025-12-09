#include <iostream>
#include <string>
#include <vector>
#include <fstream>
//#include "trade.h"
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

int add(int a, int b) {
    int sum = a + b;
    return sum;
}

int main() {
    
    int x {};
    int y {};
    std::cout << "Enter first number" << std::endl;
    std:: cin >> x;
    std::cout << "Enter second number" << std::endl;
    std:: cin >> y;


    
    // CAPTURE the data coming out of the function
    int total = add(x, y); 
    
    std::cout << "The total is: " << total << std::endl;
    return 0;
}



int main (){


    int number = 10;
    int* ptr = &number;

    std::cout << number << std::endl ;

    *ptr = 20;

    std::cout << number ;

    return 0;
}

struct Student {
    std::string name;       //std::getline(std::cin, student1.name); for space broke prevention
    int age;
    double gpa;
};

int main() {



Student student1;
std::cout << "Type students name:" << std::endl;
std::cin >> student1.name;
std::cout << "Type students age:" << std::endl;
std::cin >> student1.age;
std::cout << "Type students gpa:" << std::endl;
std::cin >> student1.gpa;

std::cout << "-----ID-CARD----" << std::endl;
std::cout << "Name: " << student1.name << std::endl;
std::cout << "Age: " << student1.age << std::endl;
std::cout << "Gpa: " << student1.gpa << std::endl;
std::cout << "----------------" << std::endl;




return 0;

}






void show_menu(){
    std::cout << "********** MENU **********" << std::endl;
    std::cout << "1. Check Balance" << std::endl;
    std::cout << "2. Deposit" << std::endl;
    std::cout << "3. Withdraw" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "**************************" << std::endl;
}

int main() {
    double balance = 500.00;
    int option;
    
    

    while (true){
        show_menu();
        std::cout << "Please select your choice:" << std::endl;
        std::cin >> option;

        if (option == 1) {
            std::cout << "Your balance is:" << balance << std::endl;
        }
        else if(option == 2){
            double add {};
            std::cout << "Choose amout of balance you want to add:" << std::endl;
            std::cin >> add;
            balance = balance + add;
             std::cout << "Your new balance is:" << balance << std::endl;
        }
        else if (option == 3){
            double take {};
            std::cout << "Yourcurrent balance is:" << balance << std::endl;
            std::cout << "Choose amout of balance you want to withdrawl:" << std::endl;
            std::cin >> take;
            if (take > balance){
                 std::cout << "Insufficient Funds" << std::endl;
            }
            else {
            balance = balance - take;
            std::cout << "Your new balance is:" << balance << std::endl;
        }}
        else if (option == 4){
            break;
        }



    }
    
    return 0;
}




class Rectangle {
public: // This makes the data accessible
    double width {};
    double height {};

    double get_area(){
        double area = width * height;
        return area; // returns the area calculated if called
    }
};
int main (){

    Rectangle rect;
    rect.width = 5;
    rect.height = 10;

    std::cout << "Area: " << rect.get_area() << std::endl;
    
return 0;
}



class Account {
    private:
    double balance{};

    public:
    void deposit(double amount){
        if (amount > 0){
            balance = balance + amount;
        }
        else {
            std::cout << "Invalid amount"<< std::endl;
        }
        
    }
        double get_balance(){
        return balance;
    }
};
main(){

    Account my_acc;
    double temp_amount;

    

    std::cout << "Choose amount to deposit:"<< std::endl;
    std::cin >> temp_amount;
   
    my_acc.deposit(temp_amount);
    std::cout << "Current Balance: " << my_acc.get_balance() << std::endl;

    return 0;
}

class Pet{
    private:
    std::string name;
    int hunger {};
    int energy = 100;

    public:
    void feed(){
        hunger = hunger - 10;
    }
    void play(){
        hunger = hunger + 10;
        energy = energy - 20;
    }
    void sleep(){
        energy = 100;
    }
    void check_status(){
        std::cout << "---PetStats---" << std::endl;
        std::cout << "Pet name:" << name << std::endl;
        std::cout << "Hunger:" << hunger << std::endl;
        std::cout << "Energy:" << energy << std::endl;
    }
    bool is_dead(){
         if (hunger == 100 || energy == 0){
            std::cout << "Your pet ran away!";
            return true;
        }
        return false;
        }
    };


main(){

    Pet my_pet;
    int user_input{};

    while(true){
        std::cout << "---PetMenu---" << std::endl;
        std::cout << "1.Feed" <<  std::endl;
        std::cout << "2.Play" <<  std::endl;
        std::cout << "3.Sleep" <<  std::endl;
        std::cout << "4.Check Status" <<  std::endl;

        std::cin >> user_input;

        if (user_input == 1){
            my_pet.feed();
        }
        if (user_input == 2){
            my_pet.play();
        }
        if (user_input == 3){
            my_pet.sleep();
        }
        if (user_input == 4){
            my_pet.check_status();
        }
        if (my_pet.is_dead()) {
            break;
        }

    }

    return 0;
}


Syntax Cheat Sheet: You need #include <vector> at the top.
std::vector<std::string> songs; // 1. Create empty list
// 2. Add items (The list grows automatically!)
songs.push_back("Shape of You"); 
songs.push_back("Believer");
// 3. Check size
std::cout << "I have " << songs.size() << " songs.";
// 4. Access items (Just like an array)
std::cout << songs[0]; // Prints "Shape of You"
🎧 Task: The Playlist Manager
The Goal: Create a program where the user can add as many songs as they want, and then print the whole list.
Requirements:
Include: #include <vector>
Setup: Inside main, create an empty vector of strings: std::vector<std::string> playlist;
Menu Loop:
Option 1 (Add): Ask user for a song name. Use playlist.push_back(name); to add it. Print "Added!".
Option 2 (Show): Use a for loop to print every song.
Hint: Your loop condition should be i < playlist.size().
Option 3 (Exit): Break the loop.
Give it a try! This is your first step into "Dynamic Memory."

int main(){

std::vector<std::string> songs;
songs.push_back("Shape of You");
songs.push_back("Believer");

std::cout << "I have " << songs.size() << " songs.";
std::cout << songs[0];

return 0;
}

int main(){

    int option {};

    std::vector<std::string> playlist;

    while (true){

        std::cout << "--PlaylistManager--" << std::endl;
        std::cout << "1.Add song" << std::endl;
        std::cout << "2.Show songs list" << std::endl;
        std::cout << "3.Exit" << std::endl;

        std::cin >> option;
        
        if (option == 1){
            std::cout << "Enter song name:" << std::endl;
            std::string name{};
            std::cin >> name;
            playlist.push_back(name);
        }
        else if (option == 2){          // use elif instead of if to skip unwanted options (save computation)
            std::cout << "List of your songs:" << std::endl;
            for ( int i = 0;i < playlist.size();i++){
                std::cout << playlist[i] << std::endl;
        }
        }
        else if (option == 3){
            std::cout << "See ya!" << std::endl;
            break;
        }
        else  {
            std::cout << "Invalid option"<< std::endl;
        }

    }
return 0;
}


class Gladiator{
    private:
    std::string name;
    int health {};
    int damage {};

    public:

    Gladiator(std::string n, int h, int d) {
        name = n;
        health = h;
        damage = d;
    }

    void attack(Gladiator &target){
        std::cout << name << " attacks " << target.name << std::endl;
        target.health = target.health - damage;
    }
    bool is_alive(){
        if (health > 0){
            return true;
        }
        else {
            return false;
        }
    }
    void announce_winner(){
        std::cout << "The winner is: " << name << std::endl;
    }
    std::string get_name(){
        return name;
    }
    int get_health(){
        return health;
    }
    int get_damage(){
        return damage;
    }
};


int main(){

    Gladiator g1("Spartacus", 100, 20);
    Gladiator g2("Crixus", 120, 15);

    std::vector<Gladiator> army;
    std::string temp_name;
    int temp_hp;
    int temp_dmg;

    while(true){
        std::cout << "--Gladiators--" << std::endl;
        std::cout << "Choose option:" << std::endl;
        std::cout << "1.Recruit" << std::endl;
        std::cout << "2.Roll Call" << std::endl;
        std::cout << "3.Identify Champion" << std::endl;
        std::cout << "4.Exit" << std::endl;
        std::cout << "--------------" << std::endl;

        int option {};
        std::cin >> option;

        if(option == 1){
              std::cout << "Choose gladiators name:" << std::endl;
              std::cin >> temp_name;
              std::cout << "Choose gladiators health:" << std::endl;
              std::cin >> temp_hp;
              std::cout << "Choose gladiators demage:" << std::endl;
              std::cin >> temp_dmg;

            army.push_back( Gladiator(temp_name, temp_hp, temp_dmg) );
        }
        else if (option ==2){
            std::cout << "--- Roster ---" << std::endl;
            for (int i = 0; i < army.size(); i++) {
                std::cout << "Fighter: " << army[i].get_name() <<std::endl;
                std::cout << "Health: " << army[i].get_health() <<std::endl;
                std::cout << "Damage: " << army[i].get_damage() <<std::endl;
            }}
        else if (option == 3) {
            if (army.size() == 0) {
                std::cout << "No fighters recruited yet!" << std::endl;
            } else {
                // "King of the Hill" Logic
                int max_dmg = -1;
                std::string strongest_name = "";

                for (int i = 0; i < army.size(); i++) {
                    // Check if current fighter is stronger than the current record
                    if (army[i].get_damage() > max_dmg) {
                        max_dmg = army[i].get_damage();
                        strongest_name = army[i].get_name();
                    }
                }
                std::cout << "\nThe Champion is: " << strongest_name << std::endl;
                std::cout << "With damage: " << max_dmg << std::endl;
            }
        }
        else if (option == 4) {
            std::cout << "Guild closed. Goodbye!" << std::endl;
            break;
        }
        else {
            std::cout << "Invalid option, try again." << std::endl;
        }
    }



    return 0;
}



void saveLedger(std::vector<Trade>& ledger){
    std::ofstream myFile("trade_history.csv"); 
    if (myFile.is_open()) {
        for(int i = 0; i < ledger.size(); i++) {
            // Note: Compiler knows getDataForFile exists because of "Trade.h"
            myFile << ledger[i].getDataForFile() << std::endl;
        }
        myFile.close(); 
    } else {
        std::cout << "Error: Could not open file." << std::endl;
    }
}

int main () {
    std::vector<Trade> dailyLedger;
    std::string temp_symbol;
    int temp_price{};
    int temp_quantity{};

    for ( int i = 0; i < 2; i++){
        std::cout << "Enter symbol" << std::endl;
        std::cin >> temp_symbol;
        std::cout << "price" << std::endl;
        std::cin >> temp_price;
        std::cout << "Enter quantity" << std::endl;
        std::cin >> temp_quantity ;

        Trade newTrade(temp_symbol, temp_price, temp_quantity);
        dailyLedger.push_back(newTrade);
    }
    
    saveLedger(dailyLedger);
    return 0;
}

*/

int main(){

int wallet = 500;

int* ptr =  &wallet;

std::cout << "Wallet ammount:" << wallet << std::endl;
std::cout << "Wallet adress:" << &wallet << std::endl;

*ptr = 1000;

std::cout << "Wallet ammount:" << wallet << std::endl;
std::cout << "Wallet adress:" << &wallet;

return 0;
}

#include <iostream>
#include <string>

int main() {
/*
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
*/

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


}
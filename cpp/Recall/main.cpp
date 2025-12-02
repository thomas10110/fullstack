#include <iostream>
#include <string>

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
std::cout << "----------------------------------" << std::endl;
std::cout << "RECEIPT:" << std::endl;
std::cout << "Item:" << item << std::endl;
std::cout << "Quantity:" << quantity << std::endl;
std::cout << "Total Cost:" << total_cost << std::endl;
std::cout << "----------------------------------" << std::endl;


return 0;

}
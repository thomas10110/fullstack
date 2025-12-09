#include "Trade.h" // We must include the menu so we know what we are cooking!
#include <string>

// The Constructor Logic
Trade::Trade(std::string s, double p, int q) {
    // 1. RE-WRITE your constructor logic here (symbol = s; etc.)
    symbol = s;
    price = p;
    quantity = q;

}
std::string Trade::getDataForFile() {
    return symbol + "," + std::to_string(price) + "," + std::to_string(quantity);
}
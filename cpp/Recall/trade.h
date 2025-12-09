#pragma once
#include <string>

class Trade {
private:
    std::string symbol;
    double price;
    int quantity;

public:
    // We just list the Constructor here. No code inside.
    Trade(std::string s, double p, int q);

    // We just list the function here. No code inside.
    std::string getDataForFile();
};
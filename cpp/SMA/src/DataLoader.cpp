#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>


struct PriceRow{
    std::string date;
    double open;
    double high;
    double low;
    double close;
    int volume;
}

{std::vector<PriceRow> loadCSV(const std::string& filepath) {
    std::vector<PriceRow> data;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: cannot open file: " << filepath << "\n";
        return data; // Return empty vector if file cannot be opened
    }

    std::string line;

    return 0;}


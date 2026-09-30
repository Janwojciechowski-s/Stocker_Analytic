#include "FileDataProvider.h"
#include <fstream>
#include <sstream>
#include <algorithm>

DataResult FileDataProvider::get_data(const std::string& csv_data)
{
    DataResult stock_data;
    std::string temp_line;

    std::stringstream input(csv_data);

    if (input.str().empty()) { 
        stock_data.warnings.push_back("Error: received empty data string.");
        return stock_data;
    }

    if (!std::getline(input, temp_line)) {
        stock_data.warnings.push_back("Data stream is empty or invalid.");
        return stock_data;
    }

    if (input.peek() == std::stringstream::traits_type::eof()) {
        stock_data.warnings.push_back("Data contains only header.");
        return stock_data;
    }

    int line_num = 1;

    while (std::getline(input, temp_line))
    {   
        ++line_num;
        if (temp_line.empty()) continue;

        std::string temp_help;
        std::stringstream ss(temp_line);

        StockRecord temp_stockrecord;
        try {
            if (!std::getline(ss, temp_stockrecord.date, ',')) throw std::runtime_error("Missing Date");

            if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing Open");
            temp_stockrecord.open = std::stod(temp_help);

            if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing High");
            temp_stockrecord.high = std::stod(temp_help);

            if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing Low");
            temp_stockrecord.low = std::stod(temp_help);

            if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing Close");
            temp_stockrecord.close = std::stod(temp_help);

            if (!std::getline(ss, temp_help)) throw std::runtime_error("Missing Volume");
            temp_help.erase(std::remove(temp_help.begin(), temp_help.end(), '\r'), temp_help.end());
            temp_stockrecord.volume = std::stoll(temp_help);

            stock_data.records.push_back(temp_stockrecord);

        }
        catch (const std::exception& e) {
        std::string warning = "Line " + std::to_string(line_num) + " error: " + e.what();
        stock_data.warnings.push_back(warning);
        }
    }
    return stock_data;
}

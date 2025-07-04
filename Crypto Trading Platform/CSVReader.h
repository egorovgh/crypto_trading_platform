#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include <string>

class CSVReader
{
    public:
        CSVReader();
        static std::vector<OrderBookEntry> readCSV(std::string csvFile);
        static std::vector<std::string> tokenise(std::string csvLine, char separator);
       
        static OrderBookEntry stringsToOrderBookEntry(std::string price, std::string amount, std::string timestamp, std::string product, OrderBookType ordeerBookType);

    private:
        static OrderBookEntry stringsToOrderBookEntry(std::vector<std::string> strings);
};

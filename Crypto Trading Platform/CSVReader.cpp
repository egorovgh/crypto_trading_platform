#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader() 
{
    
}


std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFileName)
{
    std::vector<OrderBookEntry> entries;
    std::ifstream csvFile{ csvFileName };
    std::string line;

    if (csvFile.is_open()) 
    {
        while (std::getline(csvFile, line)) 
        {
            try {
                OrderBookEntry obe = stringsToOrderBookEntry(tokenise(line, ','));
                entries.push_back(obe);
            }
            catch (const std::exception& e) {
                std::cout << "Bad data!" << std::endl;
            }
        }
    }
    else { std::cout << "File does not opened" << std::endl; }
    std::cout << "CSVReader::readCSV " << entries.size() << " entries" << std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;

    int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);

    do {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;

        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);

        tokens.push_back(token);
        start = end + 1;

    } while (end != std::string::npos); //instead of (end > 0)

    return tokens;
}

OrderBookEntry CSVReader::stringsToOrderBookEntry(std::vector<std::string> tokens)
{
    double price;
    double amount;


    if (tokens.size() != 5)       // bad
    {
        std::cout << "Bad line" << std::endl;
        throw std::exception{};
    }

    try {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (const std::exception& e) {
        throw;
    };

    OrderBookEntry obe{price, amount, tokens[0], tokens[1], OrderBookEntry::stringToOrderBookType(tokens[2])};
    return obe;
}

OrderBookEntry CSVReader::stringsToOrderBookEntry(std::string priceString, 
                                                  std::string amountString, 
                                                  std::string timestamp, 
                                                  std::string product, 
                                                  OrderBookType orderType)
{
    double price;
    double amount;
    try {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (const std::exception& e) {
        throw;
    };
    OrderBookEntry obe{price, amount, timestamp, product, orderType};
    return obe;
}
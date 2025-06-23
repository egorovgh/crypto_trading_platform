#include <iostream>
#include "MerkelMain.h"
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"


MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 1000);

    while (true) 
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    std::cout << std::endl;
    std::cout << "1: Print help" << std::endl;
    std::cout << "2: Print exchange stats" << std::endl;
    std::cout << "3: Make an ask" << std::endl;
    std::cout << "4: Make a bid" << std::endl;
    std::cout << "5: Print wallet" << std::endl;
    std::cout << "6: Continue" << std::endl;

    std::cout << "==================" << std::endl; //separator

    std::cout << "Current time is " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your goal is to make money. Analyse the market and make bids and offers." << std::endl;
}

void MerkelMain::printExchangeStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);

        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product, price, amount, eg. ETH/BTC, 200, 0.5" << std::endl;
    std::string input;

    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3) 
    {
        std::cout << "Bad input! " << std::endl;
    }
    else 
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOrderBookEntry(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::ask);
            obe.username = "simuser";   
            if (wallet.canFullfillOrder(obe)) 
            {   
                std::cout << "You made the ask!" << std::endl;
                orderBook.insertOrder(obe);
            }
            else 
            {
                std::cout << "Wallet has insufficient funds." << std::endl;
            }
        }
        catch (const std::exception& e) 
        {
            std::cout << "Bad input! " << std::endl;
        }
       
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make a bid - enter the amount: product, price, amount, eg. ETH/BTC, 200, 0.5" << std::endl;
    std::string input;

    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Bad input! " << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOrderBookEntry(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::bid);
            obe.username = "simuser";
            if (wallet.canFullfillOrder(obe))
            {
                std::cout << "You made the bid!" << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds." << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Bad input! " << std::endl;
        }

    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeFrame()
{
    std::cout << "Going to next time frame." << std::endl;



    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry& sale : sales) 
    {
        std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
        if (sale.username == "simuser")
        {
            //update the wallet
            wallet.processSale(sale);
        }
    }



    currentTime = orderBook.getNextTime(currentTime);
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;

    std::cout << "Type 1-6" << std::endl;

    std::getline(std::cin, line);
    try 
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception& e) 
    {
        //
    }


    std::cout << "You chose " << userOption << std::endl;

    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0 || userOption > 6) { //Bad input
        std::cout << "Invalid choice. Choose 1-6." << std::endl;
    }

    if (userOption == 1) {
        printHelp();
    }

    if (userOption == 2) {
        printExchangeStats();
    }

    if (userOption == 3) {
        enterAsk();
    }

    if (userOption == 4) {
        enterBid();
    }

    if (userOption == 5) {
        printWallet();
    }

    if (userOption == 6) {
        gotoNextTimeFrame();
    }
}
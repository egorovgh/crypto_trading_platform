#include "Wallet.h"
#include <string>
#include <vector>
#include "CSVReader.h"




Wallet::Wallet() 
{

}


void Wallet::insertCurrency(std::string type, double amount) 
{
    double balance;
    if (amount < 0) 
    {
        throw std::exception{};
    }

    if (currencies.count(type) == 0) //not there yet
    {
        balance = 0;
    }
    else //is there
    {
        balance = currencies[type];
    }
    balance += amount;
    currencies[type] = balance;
}


bool Wallet::removeCurrency(std::string type, double amount)
{
    if (amount < 0)
    {
        return false;
    }

    if (currencies.count(type) == 0) //not there yet
    {
        return false;
    }
    else //is there - do we have enough
    {
        if (containsCurrency(type, amount)) 
        {
            currencies[type] -= amount;
            return true;
        }
        else //they have it but not enough
        {
            return false;
        }
    }
}


bool Wallet::containsCurrency(std::string type, double amount) 
{   
    if (currencies.count(type) == 0)
        return false;
    else 
        return currencies[type] >= amount;
}


std::string Wallet::toString() 
{
    std::string s;

    for (std::pair<std::string, double> pair : currencies) 
    {
        std::string currencyName = pair.first;
        double amount = pair.second;
        s += currencyName + " : " + std::to_string(amount) + "\n";
    }

    if (s == "") 
    {
        s = "Your wallet is empty";
    }
    return s;
}


bool Wallet::canFullfillOrder(OrderBookEntry order)
{
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
    //ask
    if (order.orderType == OrderBookType::ask) 
    {
        double amount = order.amount;
        std::string currency = currs[0];
        return containsCurrency(currency, amount);
    }

    //bid
    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        std::string currency = currs[1];
        return containsCurrency(currency, amount);
    }

    return false;
}

void Wallet::processSale(OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    //ask sale
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }

    //bid sale
    if (sale.orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}



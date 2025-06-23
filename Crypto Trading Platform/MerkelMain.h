#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"


class MerkelMain 
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();

    private:
        void printMenu();
        void printHelp();
        void printExchangeStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeFrame();
        int getUserOption();
        void processUserOption(int userOption);

        std::string currentTime;

        OrderBook orderBook{"Database.csv"};

        Wallet wallet;
};
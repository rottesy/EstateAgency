#ifndef ESTATE_AGENCY_H
#define ESTATE_AGENCY_H

#include "../services/AuctionManager.h"
#include "../services/ClientManager.h"
#include "../services/PropertyManager.h"
#include "../services/TransactionManager.h"
#include <string>

class EstateAgency
{
  private:
    static EstateAgency *instance;
    PropertyManager propertyManager;
    ClientManager clientManager;
    TransactionManager transactionManager;
    AuctionManager auctionManager;
    std::string dataDirectory = "data";

    EstateAgency();

  public:
    ~EstateAgency();

    static EstateAgency *getInstance();
    static void destroyInstance();

    PropertyManager &getPropertyManager() { return propertyManager; }
    ClientManager &getClientManager() { return clientManager; }
    TransactionManager &getTransactionManager() { return transactionManager; }
    AuctionManager &getAuctionManager() { return auctionManager; }

    void saveAllData();
    void loadAllData();

    void setDataDirectory(const std::string &dir) { dataDirectory = dir; }
    std::string getDataDirectory() const { return dataDirectory; }
};

#endif

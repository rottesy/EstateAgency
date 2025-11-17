#include "../../include/core/EstateAgency.h"
#include "../../include/services/FileManager.h"
#include <filesystem>

namespace
{
constexpr const char *DATA_DIRECTORY = "data";
constexpr const char *PROPERTIES_FILE = "properties.txt";
constexpr const char *CLIENTS_FILE = "clients.txt";
constexpr const char *TRANSACTIONS_FILE = "transactions.txt";
constexpr const char *AUCTIONS_FILE = "auctions.txt";
} // namespace

EstateAgency *EstateAgency::instance = nullptr;

EstateAgency::EstateAgency() : dataDirectory(DATA_DIRECTORY)
{
    try
    {
        std::filesystem::create_directories(dataDirectory);
    }
    catch (const std::exception &)
    {
    }
}

EstateAgency::~EstateAgency() {}

EstateAgency *EstateAgency::getInstance()
{
    if (instance == nullptr)
    {
        instance = new EstateAgency();
    }
    return instance;
}

void EstateAgency::destroyInstance()
{
    if (instance != nullptr)
    {
        instance->saveAllData();
        delete instance;
        instance = nullptr;
    }
}

void EstateAgency::saveAllData() const
{
    try
    {
        FileManager::saveProperties(const_cast<PropertyManager &>(propertyManager),
                                    dataDirectory + "/" + PROPERTIES_FILE);
        FileManager::saveClients(const_cast<ClientManager &>(clientManager), dataDirectory + "/" + CLIENTS_FILE);
        FileManager::saveTransactions(const_cast<TransactionManager &>(transactionManager),
                                      dataDirectory + "/" + TRANSACTIONS_FILE);
        FileManager::saveAuctions(const_cast<AuctionManager &>(auctionManager), dataDirectory + "/" + AUCTIONS_FILE);
    }
    catch (const std::exception &e)
    {
    }
}

void EstateAgency::loadAllData()
{
    try
    {
        FileManager::loadProperties(propertyManager, dataDirectory + "/" + PROPERTIES_FILE);
        FileManager::loadClients(clientManager, dataDirectory + "/" + CLIENTS_FILE);
        FileManager::loadTransactions(transactionManager, dataDirectory + "/" + TRANSACTIONS_FILE);
        FileManager::loadAuctions(auctionManager, dataDirectory + "/" + AUCTIONS_FILE);
    }
    catch (const std::exception &e)
    {
    }
}

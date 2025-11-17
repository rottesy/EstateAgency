#include "../../include/core/EstateAgency.h"
#include "../../include/services/FileManager.h"
#include <filesystem>

namespace
{
constexpr const char *PROPERTIES_FILE = "properties.txt";
constexpr const char *CLIENTS_FILE = "clients.txt";
constexpr const char *TRANSACTIONS_FILE = "transactions.txt";
constexpr const char *AUCTIONS_FILE = "auctions.txt";
} // namespace

EstateAgency *EstateAgency::instance = nullptr;

EstateAgency::EstateAgency()
{
    try
    {
        std::filesystem::create_directories(dataDirectory);
    }
    catch (const std::filesystem::filesystem_error &)
    {
        // Ignore filesystem errors during initialization
    }
}

EstateAgency::~EstateAgency() = default;

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

void EstateAgency::saveAllData()
{
    try
    {
        FileManager::saveProperties(propertyManager, dataDirectory + "/" + PROPERTIES_FILE);
        FileManager::saveClients(clientManager, dataDirectory + "/" + CLIENTS_FILE);
        FileManager::saveTransactions(transactionManager, dataDirectory + "/" + TRANSACTIONS_FILE);
        FileManager::saveAuctions(auctionManager, dataDirectory + "/" + AUCTIONS_FILE);
    }
    catch (const FileManagerException &)
    {
        // Ignore file manager errors during save
    }
    catch (const std::filesystem::filesystem_error &)
    {
        // Ignore filesystem errors during save
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
    catch (const FileManagerException &)
    {
        // Ignore file manager errors during load
    }
    catch (const std::filesystem::filesystem_error &)
    {
        // Ignore filesystem errors during load
    }
}

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../entities/Apartment.h"
#include "../entities/Auction.h"
#include "../entities/Bid.h"
#include "../entities/Client.h"
#include "../entities/CommercialProperty.h"
#include "../entities/House.h"
#include "../entities/Property.h"
#include "../entities/Transaction.h"
#include "../services/AuctionManager.h"
#include "../services/ClientManager.h"
#include "../services/PropertyManager.h"
#include "../services/TransactionManager.h"
#include <memory>
#include <string>
#include <vector>


class FileManagerException : public std::exception
{
  private:
    std::string message;

  public:
    explicit FileManagerException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

class FileManager
{
  private:
    static constexpr char FILE_DELIMITER = '|';
    static constexpr char AVAILABLE_CHAR = '1';
    static constexpr char UNAVAILABLE_CHAR = '0';
    static constexpr size_t BID_PREFIX_LENGTH = 4;

  public:
    // Сохранение данных через менеджеры
    static void saveProperties(PropertyManager &manager, const std::string &filename);
    static void saveClients(ClientManager &manager, const std::string &filename);
    static void saveTransactions(TransactionManager &manager, const std::string &filename);
    static void saveAuctions(AuctionManager &manager, const std::string &filename);

    // Загрузка данных через менеджеры
    static void loadProperties(PropertyManager &manager, const std::string &filename);
    static void loadClients(ClientManager &manager, const std::string &filename);
    static void loadTransactions(TransactionManager &manager, const std::string &filename);
    static void loadAuctions(AuctionManager &manager, const std::string &filename);
};

#endif

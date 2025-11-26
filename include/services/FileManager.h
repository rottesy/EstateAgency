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
#include <string_view>
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

    static void parseBidLine(std::string_view line, Auction *currentAuction);
    static std::shared_ptr<Auction> parseAuctionLine(std::string_view line);

  public:
    static void saveProperties(const PropertyManager &manager, const std::string &filename);
    static void saveClients(const ClientManager &manager, const std::string &filename);
    static void saveTransactions(const TransactionManager &manager, const std::string &filename);
    static void saveAuctions(const AuctionManager &manager, const std::string &filename);

    static void loadProperties(PropertyManager &manager, const std::string &filename);
    static void loadClients(ClientManager &manager, const std::string &filename);
    static void loadTransactions(TransactionManager &manager, const std::string &filename);
    static void loadAuctions(AuctionManager &manager, const std::string &filename);
};

#endif

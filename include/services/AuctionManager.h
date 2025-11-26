#ifndef AUCTION_MANAGER_H
#define AUCTION_MANAGER_H

#include "../entities/Auction.h"
#include <memory>
#include <string_view>
#include <vector>

class AuctionManagerException : public std::exception
{
  private:
    std::string message;

  public:
    explicit AuctionManagerException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

class AuctionManager
{
  private:
    std::vector<std::shared_ptr<Auction>> auctions;

  public:
    AuctionManager();

    void addAuction(std::shared_ptr<Auction> auction);
    bool removeAuction(const std::string &id);
    Auction *findAuction(const std::string &id) const;

    std::vector<Auction *> getAllAuctions() const;
    std::vector<Auction *> getActiveAuctions() const;
    std::vector<Auction *> getCompletedAuctions() const;
    std::vector<Auction *> getAuctionsByProperty(std::string_view propertyId) const;

    const std::vector<std::shared_ptr<Auction>> &getAuctions() const { return auctions; }
    void setAuctions(std::vector<std::shared_ptr<Auction>> &&newAuctions) { auctions = std::move(newAuctions); }

    size_t getCount() const { return auctions.size(); }
};

#endif

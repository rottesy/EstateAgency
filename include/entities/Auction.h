#ifndef AUCTION_H
#define AUCTION_H

#include "Bid.h"
#include "Property.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Auction
{
  private:
    std::string id;
    std::string propertyId;
    std::string propertyAddress;
    double startingPrice;
    double buyoutPrice;
    std::vector<std::shared_ptr<Bid>> bids;
    std::string status;
    std::string createdAt;
    std::string completedAt;

  public:
    Auction(const std::string &id, const std::string &propertyId, const std::string &propertyAddress,
            double startingPrice);

    friend std::ostream &operator<<(std::ostream &os, const Auction &auction);

    bool operator==(const Auction &other) const;
    bool operator<(const Auction &other) const;

    bool addBid(std::shared_ptr<Bid> bid);
    void addBidDirect(std::shared_ptr<Bid> bid);

    double getCurrentHighestBid() const;
    Bid *getHighestBid() const;

    void complete();
    void cancel();

    std::string getId() const { return id; }
    std::string getPropertyId() const { return propertyId; }
    std::string getPropertyAddress() const { return propertyAddress; }
    double getStartingPrice() const { return startingPrice; }
    double getBuyoutPrice() const { return buyoutPrice; }
    std::vector<std::shared_ptr<Bid>> getBids() const { return bids; }
    std::string getStatus() const { return status; }
    std::string getCreatedAt() const { return createdAt; }
    std::string getCompletedAt() const { return completedAt; }
    bool isActive() const { return status == "active"; }
    bool isCompleted() const { return status == "completed"; }

    bool wasBuyout() const;
    std::string toString() const;
    std::string toFileString() const;
};

std::ostream &operator<<(std::ostream &os, const Auction &auction);

#endif


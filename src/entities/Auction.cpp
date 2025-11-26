#include "../../include/entities/Auction.h"
#include "../../include/core/Constants.h"
#include "../../include/core/Utils.h"
#include <compare>
#include <format>
#include <stdexcept>

namespace
{
constexpr double MIN_PRICE = 0.0;
constexpr double BUYOUT_MULTIPLIER = 1.7;
constexpr double MIN_BID_INCREMENT = 0.01;
constexpr double NO_BID = 0.0;
constexpr const char *const DATE_FORMAT = "%Y-%m-%d %H:%M:%S";
constexpr int PRICE_PRECISION = 2;
} 

Auction::Auction(const std::string &id, const std::string &propertyId, const std::string &propertyAddress,
                 double startingPrice)
    : id(id), propertyId(propertyId), propertyAddress(propertyAddress), startingPrice(startingPrice),
      buyoutPrice(startingPrice * BUYOUT_MULTIPLIER)
{
    if (startingPrice <= MIN_PRICE)
    {
        throw std::invalid_argument("Starting price must be positive");
    }

    createdAt = Utils::getCurrentTimeString(DATE_FORMAT);
}

bool Auction::operator==(const Auction &other) const { return id == other.id; }

std::strong_ordering Auction::operator<=>(const Auction &other) const
{
    if (auto cmp = createdAt <=> other.createdAt; cmp != 0)
        return cmp;
    return id <=> other.id;
}

bool Auction::addBid(std::shared_ptr<Bid> bid)
{
    if (status != Constants::AuctionStatus::ACTIVE)
    {
        return false;
    }

    double currentHighest = getCurrentHighestBid();

    if (bid->getAmount() >= buyoutPrice)
    {
        bids.push_back(bid);
        complete();
        return true;
    }

    if (double minBid = (currentHighest > NO_BID) ? currentHighest + MIN_BID_INCREMENT : startingPrice;
        bid->getAmount() < minBid)
    {
        return false;
    }

    bids.push_back(bid);
    return true;
}

void Auction::addBidDirect(std::shared_ptr<Bid> bid) { bids.push_back(bid); }

double Auction::getCurrentHighestBid() const
{
    if (bids.empty())
    {
        return NO_BID;
    }

    double maxBid = NO_BID;
    for (const auto &bid : bids)
    {
        if (bid->getAmount() > maxBid)
        {
            maxBid = bid->getAmount();
        }
    }
    return maxBid;
}

const Bid *Auction::getHighestBid() const
{
    if (bids.empty())
    {
        return nullptr;
    }

    const Bid *highest = nullptr;
    double maxBid = NO_BID;
    for (const auto &bid : bids)
    {
        if (bid->getAmount() > maxBid)
        {
            maxBid = bid->getAmount();
            highest = bid.get();
        }
    }
    return highest;
}

void Auction::complete()
{
    if (status == Constants::AuctionStatus::ACTIVE)
    {
        status = Constants::AuctionStatus::COMPLETED;
        completedAt = Utils::getCurrentTimeString(DATE_FORMAT);
    }
}

void Auction::cancel()
{
    if (status == Constants::AuctionStatus::ACTIVE)
    {
        status = Constants::AuctionStatus::CANCELLED;
        completedAt = Utils::getCurrentTimeString(DATE_FORMAT);
    }
}

bool Auction::wasBuyout() const
{
    if (!isCompleted() || bids.empty())
    {
        return false;
    }

    const Bid *highest = getHighestBid();
    return highest != nullptr && highest->getAmount() >= buyoutPrice;
}

std::string Auction::toString() const
{
    return std::format("Auction ID: {}, Property: {}, Starting: {:.2f} руб., Status: {}, Bids: {}", id, propertyAddress,
                       startingPrice, status, bids.size());
}

std::string Auction::toFileString() const
{
    return std::format("{}|{}|{}|{:.2f}|{:.2f}|{}|{}|{}", id, propertyId, propertyAddress, startingPrice, buyoutPrice,
                       status, createdAt, completedAt);
}

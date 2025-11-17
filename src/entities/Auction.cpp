#include "../../include/entities/Auction.h"
#include "../../include/core/Constants.h"
#include "../../include/core/Utils.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace
{
constexpr double MIN_PRICE = 0.0;
constexpr double BUYOUT_MULTIPLIER = 1.7;
constexpr double MIN_BID_INCREMENT = 0.01;
constexpr double NO_BID = 0.0;
constexpr const char *const DATE_FORMAT = "%Y-%m-%d %H:%M:%S";
constexpr int PRICE_PRECISION = 2;
} // namespace

Auction::Auction(const std::string &id, const std::string &propertyId, const std::string &propertyAddress,
                 double startingPrice)
    : id(id), propertyId(propertyId), propertyAddress(propertyAddress), startingPrice(startingPrice),
      buyoutPrice(startingPrice * BUYOUT_MULTIPLIER)
{
    if (startingPrice <= MIN_PRICE)
    {
        throw std::invalid_argument("Starting price must be positive");
    }

    auto now = std::time(nullptr);
    auto tm = Utils::getLocalTime(now);
    std::ostringstream oss;
    oss << std::put_time(&tm, DATE_FORMAT);
    createdAt = oss.str();
}

bool Auction::operator==(const Auction &other) const { return id == other.id; }

bool Auction::operator<(const Auction &other) const { return createdAt < other.createdAt; }

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

    double minBid = (currentHighest > NO_BID) ? currentHighest + MIN_BID_INCREMENT : startingPrice;
    if (bid->getAmount() < minBid)
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

Bid *Auction::getHighestBid() const
{
    if (bids.empty())
    {
        return nullptr;
    }

    Bid *highest = nullptr;
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
        auto now = std::time(nullptr);
        auto tm = Utils::getLocalTime(now);
        std::ostringstream oss;
        oss << std::put_time(&tm, DATE_FORMAT);
        completedAt = oss.str();
    }
}

void Auction::cancel()
{
    if (status == Constants::AuctionStatus::ACTIVE)
    {
        status = Constants::AuctionStatus::CANCELLED;
        auto now = std::time(nullptr);
        auto tm = Utils::getLocalTime(now);
        std::ostringstream oss;
        oss << std::put_time(&tm, DATE_FORMAT);
        completedAt = oss.str();
    }
}

bool Auction::wasBuyout() const
{
    if (!isCompleted() || bids.empty())
    {
        return false;
    }

    Bid *highest = getHighestBid();
    return highest != nullptr && highest->getAmount() >= buyoutPrice;
}

std::string Auction::toString() const
{
    std::ostringstream oss;
    oss << "Auction ID: " << id << ", "
        << "Property: " << propertyAddress << ", "
        << "Starting: " << std::fixed << std::setprecision(PRICE_PRECISION) << startingPrice << " руб., "
        << "Status: " << status << ", "
        << "Bids: " << bids.size();
    return oss.str();
}

std::string Auction::toFileString() const
{
    std::ostringstream oss;
    oss << id << "|" << propertyId << "|" << propertyAddress << "|" << std::fixed << std::setprecision(PRICE_PRECISION)
        << startingPrice << "|" << buyoutPrice << "|" << status << "|" << createdAt << "|" << completedAt;
    return oss.str();
}

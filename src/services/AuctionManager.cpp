#include "../../include/services/AuctionManager.h"
#include <algorithm>

AuctionManager::AuctionManager() {}

void AuctionManager::addAuction(std::shared_ptr<Auction> auction)
{
    if (!auction)
    {
        throw AuctionManagerException("Cannot add null auction");
    }
    if (findAuction(auction->getId()) != nullptr)
    {
        throw AuctionManagerException("Auction with ID " + auction->getId() + " already exists");
    }
    auctions.push_back(auction);
}

bool AuctionManager::removeAuction(const std::string &id)
{
    auto it = std::remove_if(auctions.begin(), auctions.end(),
                             [&id](const std::shared_ptr<Auction> &auction) { return auction->getId() == id; });

    if (it != auctions.end())
    {
        auctions.erase(it, auctions.end());
        return true;
    }
    return false;
}

Auction *AuctionManager::findAuction(const std::string &id) const
{
    auto it = std::find_if(auctions.begin(), auctions.end(),
                           [&id](const std::shared_ptr<Auction> &auction) { return auction->getId() == id; });

    return (it != auctions.end()) ? it->get() : nullptr;
}

std::vector<Auction *> AuctionManager::getAllAuctions() const
{
    std::vector<Auction *> result;
    result.reserve(auctions.size());
    for (const auto &auction : auctions)
    {
        result.push_back(auction.get());
    }
    return result;
}

std::vector<Auction *> AuctionManager::getActiveAuctions() const
{
    std::vector<Auction *> result;
    for (const auto &auction : auctions)
    {
        if (auction->isActive())
        {
            result.push_back(auction.get());
        }
    }
    return result;
}

std::vector<Auction *> AuctionManager::getCompletedAuctions() const
{
    std::vector<Auction *> result;
    for (const auto &auction : auctions)
    {
        if (auction->isCompleted())
        {
            result.push_back(auction.get());
        }
    }
    return result;
}

std::vector<Auction *> AuctionManager::getAuctionsByProperty(const std::string &propertyId) const
{
    std::vector<Auction *> result;
    for (const auto &auction : auctions)
    {
        if (auction->getPropertyId() == propertyId)
        {
            result.push_back(auction.get());
        }
    }
    return result;
}


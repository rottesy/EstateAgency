#include "../../include/services/AuctionManager.h"
#include <algorithm>
#include <ranges>
#include <string_view>

AuctionManager::AuctionManager() = default;

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
    if (auto removed = std::ranges::remove_if(auctions, [&id](const std::shared_ptr<Auction> &auction)
                                              { return auction->getId() == id; });
        removed.begin() != auctions.end())
    {
        auctions.erase(removed.begin(), auctions.end());
        return true;
    }
    return false;
}

Auction *AuctionManager::findAuction(const std::string &id) const
{
    if (auto it = std::ranges::find_if(auctions, [&id](const std::shared_ptr<Auction> &auction)
                                       { return auction->getId() == id; });
        it != auctions.end())
    {
        return it->get();
    }
    return nullptr;
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

std::vector<Auction *> AuctionManager::getAuctionsByProperty(std::string_view propertyId) const
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

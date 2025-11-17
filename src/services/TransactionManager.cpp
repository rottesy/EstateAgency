#include "../../include/services/TransactionManager.h"
#include <algorithm>
#include <ranges>
#include <string_view>

TransactionManager::TransactionManager() = default;

void TransactionManager::addTransaction(std::shared_ptr<Transaction> transaction)
{
    if (!transaction)
    {
        throw TransactionManagerException("Cannot add null transaction");
    }
    if (findTransaction(transaction->getId()) != nullptr)
    {
        throw TransactionManagerException("Transaction with ID " + transaction->getId() + " already exists");
    }
    transactions.push_back(transaction);
}

bool TransactionManager::removeTransaction(const std::string &id)
{
    auto removed = std::ranges::remove_if(transactions, [&id](const std::shared_ptr<Transaction> &trans)
                                          { return trans->getId() == id; });
    if (removed.begin() != transactions.end())
    {
        transactions.erase(removed.begin(), transactions.end());
        return true;
    }
    return false;
}

Transaction *TransactionManager::findTransaction(const std::string &id) const
{
    if (auto it = std::ranges::find_if(transactions, [&id](const std::shared_ptr<Transaction> &trans)
                                       { return trans->getId() == id; });
        it != transactions.end())
    {
        return it->get();
    }
    return nullptr;
}

std::vector<Transaction *> TransactionManager::getAllTransactions() const
{
    std::vector<Transaction *> result;
    result.reserve(transactions.size());
    for (const auto &trans : transactions)
    {
        result.push_back(trans.get());
    }
    return result;
}

std::vector<Transaction *> TransactionManager::getTransactionsByClient(std::string_view clientId) const
{
    std::vector<Transaction *> result;
    for (const auto &trans : transactions)
    {
        if (trans->getClientId() == clientId)
        {
            result.push_back(trans.get());
        }
    }
    return result;
}

std::vector<Transaction *> TransactionManager::getTransactionsByProperty(std::string_view propertyId) const
{
    std::vector<Transaction *> result;
    for (const auto &trans : transactions)
    {
        if (trans->getPropertyId() == propertyId)
        {
            result.push_back(trans.get());
        }
    }
    return result;
}

std::vector<Transaction *> TransactionManager::getTransactionsByStatus(std::string_view status) const
{
    std::vector<Transaction *> result;
    for (const auto &trans : transactions)
    {
        if (trans->getStatus() == status)
        {
            result.push_back(trans.get());
        }
    }
    return result;
}

#include "../../include/services/TransactionManager.h"
#include <algorithm>

TransactionManager::TransactionManager() {}

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
    auto it = std::remove_if(transactions.begin(), transactions.end(),
                             [&id](const std::shared_ptr<Transaction> &trans) { return trans->getId() == id; });

    if (it != transactions.end())
    {
        transactions.erase(it, transactions.end());
        return true;
    }
    return false;
}

Transaction *TransactionManager::findTransaction(const std::string &id) const
{
    auto it = std::find_if(transactions.begin(), transactions.end(),
                           [&id](const std::shared_ptr<Transaction> &trans) { return trans->getId() == id; });

    return (it != transactions.end()) ? it->get() : nullptr;
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

std::vector<Transaction *> TransactionManager::getTransactionsByClient(const std::string &clientId) const
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

std::vector<Transaction *> TransactionManager::getTransactionsByProperty(const std::string &propertyId) const
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

std::vector<Transaction *> TransactionManager::getTransactionsByStatus(const std::string &status) const
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


#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include "../entities/Transaction.h"
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class TransactionManagerException : public std::exception
{
  private:
    std::string message;

  public:
    explicit TransactionManagerException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

class TransactionManager
{
  private:
    std::vector<std::shared_ptr<Transaction>> transactions;

  public:
    TransactionManager();

    void addTransaction(std::shared_ptr<Transaction> transaction);
    bool removeTransaction(const std::string &id);
    Transaction *findTransaction(const std::string &id) const;

    std::vector<Transaction *> getAllTransactions() const;
    std::vector<Transaction *> getTransactionsByClient(std::string_view clientId) const;
    std::vector<Transaction *> getTransactionsByProperty(std::string_view propertyId) const;
    std::vector<Transaction *> getTransactionsByStatus(std::string_view status) const;

    const std::vector<std::shared_ptr<Transaction>> &getTransactions() const { return transactions; }
    void setTransactions(std::vector<std::shared_ptr<Transaction>> &&newTransactions)
    {
        transactions = std::move(newTransactions);
    }

    size_t getCount() const { return transactions.size(); }
};

#endif

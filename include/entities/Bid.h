#ifndef BID_H
#define BID_H

#include <iomanip>
#include <iostream>
#include <string>

class Bid
{
  private:
    std::string clientId;
    std::string clientName;
    double amount;
    std::string timestamp;

  public:
    Bid(const std::string &clientId, const std::string &clientName, double amount);

    friend std::ostream &operator<<(std::ostream &os, const Bid &bid);

    bool operator==(const Bid &other) const;
    bool operator<(const Bid &other) const;

    std::string getClientId() const { return clientId; }
    std::string getClientName() const { return clientName; }
    double getAmount() const { return amount; }
    std::string getTimestamp() const { return timestamp; }

    std::string toString() const;
    std::string toFileString() const;

    friend std::ostream &operator<<(std::ostream &os, const Bid &bid)
    {
        os << "Client: " << bid.clientName << " (ID: " << bid.clientId << ")\n"
           << "Amount: " << std::fixed << std::setprecision(2) << bid.amount << " руб.\n"
           << "Time: " << bid.timestamp;
        return os;
    }
};

#endif

#ifndef BID_H
#define BID_H

#include <compare>
#include <format>
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
    std::partial_ordering operator<=>(const Bid &other) const;

    std::string getClientId() const { return clientId; }
    std::string getClientName() const { return clientName; }
    double getAmount() const { return amount; }
    std::string getTimestamp() const { return timestamp; }

    std::string toString() const;
    std::string toFileString() const;

    friend std::ostream &operator<<(std::ostream &os, const Bid &bid)
    {
        os << std::format("Client: {} (ID: {})\n", bid.clientName, bid.clientId)
           << std::format("Amount: {:.2f} руб.\n", bid.amount) << std::format("Time: {}", bid.timestamp);
        return os;
    }
};

#endif

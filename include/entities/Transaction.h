#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <compare>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

class Transaction
{
  private:
    std::string id;
    std::string propertyId;
    std::string clientId;
    std::string date;
    double finalPrice;
    std::string status;
    std::string notes;

  public:
    Transaction(const std::string &id, const std::string &propertyId, const std::string &clientId, double finalPrice,
                const std::string &status = "pending", const std::string &notes = "");

    friend std::ostream &operator<<(std::ostream &os, const Transaction &trans);

    bool operator==(const Transaction &other) const;
    std::strong_ordering operator<=>(const Transaction &other) const;

    std::string getId() const { return id; }
    std::string getPropertyId() const { return propertyId; }
    std::string getClientId() const { return clientId; }
    std::string getDate() const { return date; }
    double getFinalPrice() const { return finalPrice; }
    std::string getStatus() const { return status; }
    std::string getNotes() const { return notes; }

    void setStatus(std::string_view status);
    void setFinalPrice(double price);
    void setNotes(std::string_view notes);

    static bool validateId(std::string_view id);

    std::string toString() const;
    std::string toFileString() const;

    friend std::ostream &operator<<(std::ostream &os, const Transaction &trans)
    {
        os << std::format("Transaction ID: {}\n", trans.id) << std::format("Property ID: {}\n", trans.propertyId)
           << std::format("Client ID: {}\n", trans.clientId) << std::format("Date: {}\n", trans.date)
           << std::format("Final Price: {:.2f} руб.\n", trans.finalPrice) << std::format("Status: {}\n", trans.status)
           << std::format("Notes: {}", trans.notes);
        return os;
    }
};

#endif

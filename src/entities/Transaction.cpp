#include "../../include/entities/Transaction.h"
#include "../../include/core/Constants.h"
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace
{
constexpr size_t MIN_ID_LENGTH = 6;
constexpr size_t MAX_ID_LENGTH = 8;
constexpr double MIN_PRICE = 0.0;
constexpr const char *const DATE_FORMAT = "%Y-%m-%d %H:%M:%S";
} // namespace

Transaction::Transaction(const std::string &id, const std::string &propertyId, const std::string &clientId,
                         double finalPrice, const std::string &status, const std::string &notes)
    : id(id), propertyId(propertyId), clientId(clientId), finalPrice(finalPrice), status(status), notes(notes)
{
    if (!validateId(id))
    {
        throw std::invalid_argument("Invalid ID: must be 6-8 digits only");
    }
    if (propertyId.empty())
    {
        throw std::invalid_argument("Property ID cannot be empty");
    }
    if (clientId.empty())
    {
        throw std::invalid_argument("Client ID cannot be empty");
    }
    if (finalPrice <= MIN_PRICE)
    {
        throw std::invalid_argument("Final price must be positive");
    }
    if (status != Constants::TransactionStatus::PENDING && status != Constants::TransactionStatus::COMPLETED &&
        status != Constants::TransactionStatus::CANCELLED)
    {
        throw std::invalid_argument("Invalid status");
    }

    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, DATE_FORMAT);
    this->date = oss.str();
}

std::ostream &operator<<(std::ostream &os, const Transaction &trans)
{
    os << "Transaction ID: " << trans.id << "\n"
       << "Property ID: " << trans.propertyId << "\n"
       << "Client ID: " << trans.clientId << "\n"
       << "Date: " << trans.date << "\n"
       << "Final Price: " << std::fixed << std::setprecision(2) << trans.finalPrice << " руб.\n"
       << "Status: " << trans.status << "\n"
       << "Notes: " << trans.notes;
    return os;
}

bool Transaction::operator==(const Transaction &other) const { return id == other.id; }

bool Transaction::operator<(const Transaction &other) const { return date < other.date; }

void Transaction::setStatus(const std::string &newStatus)
{
    if (newStatus != Constants::TransactionStatus::PENDING && newStatus != Constants::TransactionStatus::COMPLETED &&
        newStatus != Constants::TransactionStatus::CANCELLED)
    {
        throw std::invalid_argument("Invalid status");
    }
    status = newStatus;
}

void Transaction::setFinalPrice(double newPrice)
{
    if (newPrice <= MIN_PRICE)
    {
        throw std::invalid_argument("Final price must be positive");
    }
    finalPrice = newPrice;
}

void Transaction::setNotes(const std::string &newNotes) { notes = newNotes; }

std::string Transaction::toString() const
{
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

std::string Transaction::toFileString() const
{
    std::ostringstream oss;
    oss << id << "|" << propertyId << "|" << clientId << "|" << date << "|" << finalPrice << "|" << status << "|"
        << notes;
    return oss.str();
}

bool Transaction::validateId(const std::string &id)
{
    if (id.empty() || id.length() < MIN_ID_LENGTH || id.length() > MAX_ID_LENGTH)
    {
        return false;
    }

    for (char c : id)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            return false;
        }
    }
    return true;
}

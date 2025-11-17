#include "../../include/entities/Bid.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace
{
constexpr double MIN_AMOUNT = 0.0;
constexpr const char *const DATE_FORMAT = "%Y-%m-%d %H:%M:%S";
constexpr int PRICE_PRECISION = 2;
} // namespace

Bid::Bid(const std::string &clientId, const std::string &clientName, double amount)
    : clientId(clientId), clientName(clientName), amount(amount)
{
    if (amount <= MIN_AMOUNT)
    {
        throw std::invalid_argument("Bid amount must be positive");
    }

    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, DATE_FORMAT);
    timestamp = oss.str();
}

bool Bid::operator==(const Bid &other) const { return clientId == other.clientId && amount == other.amount; }

bool Bid::operator<(const Bid &other) const
{
    if (amount != other.amount)
    {
        return amount < other.amount;
    }
    return timestamp < other.timestamp;
}

std::string Bid::toString() const
{
    std::ostringstream oss;
    oss << "Client: " << clientName << " (ID: " << clientId << "), "
        << "Amount: " << std::fixed << std::setprecision(PRICE_PRECISION) << amount << " руб., "
        << "Time: " << timestamp;
    return oss.str();
}

std::string Bid::toFileString() const
{
    std::ostringstream oss;
    oss << clientId << "|" << clientName << "|" << std::fixed << std::setprecision(PRICE_PRECISION) << amount << "|"
        << timestamp;
    return oss.str();
}

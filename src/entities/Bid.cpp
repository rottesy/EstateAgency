#include "../../include/entities/Bid.h"
#include "../../include/core/Utils.h"
#include <compare>
#include <format>
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

    timestamp = Utils::getCurrentTimeString(DATE_FORMAT);
}

bool Bid::operator==(const Bid &other) const { return clientId == other.clientId && amount == other.amount; }

std::partial_ordering Bid::operator<=>(const Bid &other) const
{
    if (auto cmp = amount <=> other.amount; cmp != 0)
        return cmp;
    // Convert string comparison (strong_ordering) to partial_ordering
    auto strCmp = timestamp <=> other.timestamp;
    if (strCmp == std::strong_ordering::less)
        return std::partial_ordering::less;
    if (strCmp == std::strong_ordering::greater)
        return std::partial_ordering::greater;
    return std::partial_ordering::equivalent;
}

std::string Bid::toString() const
{
    return std::format("Client: {} (ID: {}), Amount: {:.2f} руб., Time: {}", clientName, clientId, amount, timestamp);
}

std::string Bid::toFileString() const
{
    return std::format("{}|{}|{:.2f}|{}", clientId, clientName, amount, timestamp);
}

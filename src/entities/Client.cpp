#include "../../include/entities/Client.h"
#include "../../include/core/Utils.h"
#include <algorithm>
#include <cctype>
#include <compare>
#include <iomanip>
#include <ranges>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace
{
constexpr size_t MIN_ID_LENGTH = 6;
constexpr size_t MAX_ID_LENGTH = 8;
constexpr const char *const DATE_FORMAT = "%Y-%m-%d %H:%M:%S";
} 

Client::Client(const std::string &id, const std::string &name, const std::string &phone, const std::string &email)
    : id(id), name(name), phone(phone), email(email)
{
    if (!validateId(id))
    {
        throw std::invalid_argument("Invalid ID: must be 6-8 digits only");
    }
    if (!validatePhone(phone))
    {
        throw std::invalid_argument("Invalid phone number format");
    }
    if (!validateEmail(email))
    {
        throw std::invalid_argument("Invalid email format");
    }

    registrationDate = Utils::getCurrentTimeString(DATE_FORMAT);
}

bool Client::operator==(const Client &other) const { return id == other.id; }

std::strong_ordering Client::operator<=>(const Client &other) const { return name <=> other.name; }

void Client::setName(std::string_view newName)
{
    if (newName.empty())
    {
        throw std::invalid_argument("Name cannot be empty");
    }
    name = std::string(newName);
}

void Client::setPhone(std::string_view newPhone)
{
    if (!validatePhone(newPhone))
    {
        throw std::invalid_argument("Invalid phone number format");
    }
    phone = std::string(newPhone);
}

void Client::setEmail(std::string_view newEmail)
{
    if (!validateEmail(newEmail))
    {
        throw std::invalid_argument("Invalid email format");
    }
    email = std::string(newEmail);
}

bool Client::validateId(std::string_view id)
{
    if (id.empty() || id.length() < MIN_ID_LENGTH || id.length() > MAX_ID_LENGTH)
    {
        return false;
    }

    return std::ranges::all_of(id, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
}

bool Client::validatePhone(std::string_view phone)
{
    static const std::regex pattern(R"(\+375\d{9})");
    return std::regex_match(phone.begin(), phone.end(), pattern);
}

bool Client::validateEmail(std::string_view email)
{
    static const std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email.begin(), email.end(), pattern);
}

std::string Client::toString() const
{
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

#include "../../include/entities/Client.h"
#include <cctype>
#include <ctime>
#include <iomanip>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace
{
constexpr size_t MIN_ID_LENGTH = 6;
constexpr size_t MAX_ID_LENGTH = 8;
constexpr const char *const DATE_FORMAT = "%Y-%m-%d %H:%M:%S";
} // namespace

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

    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, DATE_FORMAT);
    registrationDate = oss.str();
}

bool Client::operator==(const Client &other) const { return id == other.id; }

bool Client::operator<(const Client &other) const { return name < other.name; }

void Client::setName(const std::string &newName)
{
    if (newName.empty())
    {
        throw std::invalid_argument("Name cannot be empty");
    }
    name = newName;
}

void Client::setPhone(const std::string &newPhone)
{
    if (!validatePhone(newPhone))
    {
        throw std::invalid_argument("Invalid phone number format");
    }
    phone = newPhone;
}

void Client::setEmail(const std::string &newEmail)
{
    if (!validateEmail(newEmail))
    {
        throw std::invalid_argument("Invalid email format");
    }
    email = newEmail;
}

bool Client::validateId(const std::string &id)
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

bool Client::validatePhone(const std::string &phone)
{
    static const std::regex pattern(R"(\+375\d{9})");
    return std::regex_match(phone, pattern);
}

bool Client::validateEmail(const std::string &email)
{
    static const std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, pattern);
}

std::string Client::toString() const
{
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

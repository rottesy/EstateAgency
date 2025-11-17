#include "../../include/entities/Property.h"
#include <algorithm>
#include <cctype>
#include <compare>
#include <ranges>
#include <stdexcept>
#include <string_view>

Property::Property(const std::string &id, const std::string &city, const std::string &street, const std::string &house,
                   double price, double area, const std::string &description)
    : id(id), city(city), street(street), house(house), price(price), area(area), description(description)
{
    if (!validateId(id))
    {
        throw std::invalid_argument("Invalid ID: must be 6-8 digits only");
    }
    if (!validateAddressPart(city) || city.empty())
    {
        throw std::invalid_argument("Invalid city");
    }
    if (!validateAddressPart(street) || street.empty())
    {
        throw std::invalid_argument("Invalid street");
    }
    if (!validateAddressPart(house) || house.empty())
    {
        throw std::invalid_argument("Invalid house");
    }
    if (!validatePrice(price))
    {
        throw std::invalid_argument("Invalid price: must be between 10000 and 1000000000");
    }
    if (!validateArea(area))
    {
        throw std::invalid_argument("Invalid area: must be positive");
    }
}

bool Property::operator==(const Property &other) const { return id == other.id; }

std::partial_ordering Property::operator<=>(const Property &other) const { return price <=> other.price; }

void Property::setPrice(double newPrice)
{
    if (!validatePrice(newPrice))
    {
        throw std::invalid_argument("Invalid price: must be between 10000 and 1000000000");
    }
    price = newPrice;
}

void Property::setArea(double newArea)
{
    if (!validateArea(newArea))
    {
        throw std::invalid_argument("Invalid area: must be positive");
    }
    area = newArea;
}

void Property::setAddress(const std::string &newCity, const std::string &newStreet, const std::string &newHouse)
{
    if (!validateAddressPart(newCity) || newCity.empty())
    {
        throw std::invalid_argument("Invalid city");
    }
    if (!validateAddressPart(newStreet) || newStreet.empty())
    {
        throw std::invalid_argument("Invalid street");
    }
    if (!validateAddressPart(newHouse) || newHouse.empty())
    {
        throw std::invalid_argument("Invalid house");
    }
    city = newCity;
    street = newStreet;
    house = newHouse;
}

void Property::setDescription(std::string_view newDesc) { description = std::string(newDesc); }

bool Property::validatePrice(double price)
{
    constexpr double MIN_PRICE = 10000.0;
    constexpr double MAX_PRICE = 1000000000.0;
    return price >= MIN_PRICE && price <= MAX_PRICE;
}

bool Property::validateArea(double area)
{
    constexpr double MIN_AREA = 0.0;
    constexpr double MAX_AREA = 100000.0;
    return area > MIN_AREA && area <= MAX_AREA;
}

bool Property::validateId(std::string_view id)
{
    constexpr size_t MIN_ID_LENGTH = 6;
    constexpr size_t MAX_ID_LENGTH = 8;
    if (id.empty() || id.length() < MIN_ID_LENGTH || id.length() > MAX_ID_LENGTH)
    {
        return false;
    }

    return std::ranges::all_of(id, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
}

bool Property::validateAddressPart(std::string_view part)
{
    constexpr size_t MAX_ADDRESS_LENGTH = 100;
    if (part.empty() || part.length() > MAX_ADDRESS_LENGTH)
    {
        return false;
    }

    return std::ranges::any_of(part, [](char c) { return std::isprint(static_cast<unsigned char>(c)); });
}

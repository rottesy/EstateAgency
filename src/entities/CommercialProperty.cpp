#include "../../include/entities/CommercialProperty.h"
#include <sstream>
#include <stdexcept>

namespace
{
constexpr int MIN_PARKING_SPACES = 0;
constexpr int MAX_PARKING_SPACES = 1000;
} // namespace

CommercialProperty::CommercialProperty(const std::string &id, const std::string &city, const std::string &street,
                                       const std::string &house, double price, double area,
                                       const std::string &description, const std::string &businessType, bool hasParking,
                                       int parkingSpaces, bool isVisibleFromStreet)
    : Property(id, city, street, house, price, area, description), businessType(businessType), hasParking(hasParking),
      parkingSpaces(parkingSpaces), isVisibleFromStreet(isVisibleFromStreet)
{
    if (businessType.empty())
    {
        throw std::invalid_argument("Business type cannot be empty");
    }
    if (parkingSpaces < MIN_PARKING_SPACES || parkingSpaces > MAX_PARKING_SPACES)
    {
        throw std::invalid_argument("Parking spaces must be between 0 and 1000");
    }
}

std::string CommercialProperty::getType() const { return "Commercial"; }

void CommercialProperty::displayInfo() const
{
    std::cout << *this << "\n";
    std::cout << "Business Type: " << businessType << "\n";
    std::cout << "Parking: " << (hasParking ? "Yes" : "No") << "\n";
    std::cout << "Parking Spaces: " << parkingSpaces << "\n";
    std::cout << "Visible from Street: " << (isVisibleFromStreet ? "Yes" : "No") << "\n";
}

std::string CommercialProperty::toFileString() const
{
    std::ostringstream oss;
    oss << "COMMERCIAL|" << getId() << "|" << getCity() << "|" << getStreet() << "|" << getHouse() << "|" << getPrice()
        << "|" << getArea() << "|" << getDescription() << "|" << (getIsAvailable() ? "1" : "0") << "|" << businessType
        << "|" << (hasParking ? "1" : "0") << "|" << parkingSpaces << "|" << (isVisibleFromStreet ? "1" : "0");
    return oss.str();
}

Property *CommercialProperty::clone() const
{
    return new CommercialProperty(getId(), getCity(), getStreet(), getHouse(), getPrice(), getArea(), getDescription(),
                                  businessType, hasParking, parkingSpaces, isVisibleFromStreet);
}

void CommercialProperty::setBusinessType(const std::string &type)
{
    if (type.empty())
    {
        throw std::invalid_argument("Business type cannot be empty");
    }
    businessType = type;
}

void CommercialProperty::setParkingSpaces(int spaces)
{
    if (spaces < MIN_PARKING_SPACES || spaces > MAX_PARKING_SPACES)
    {
        throw std::invalid_argument("Parking spaces must be between 0 and 1000");
    }
    parkingSpaces = spaces;
}

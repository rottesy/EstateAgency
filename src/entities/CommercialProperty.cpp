#include "../../include/entities/CommercialProperty.h"
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace
{
constexpr int MIN_PARKING_SPACES = 0;
constexpr int MAX_PARKING_SPACES = 1000;
} // namespace

CommercialProperty::CommercialProperty(const CommercialPropertyParams &params)
    : Property(params.base.id, params.base.city, params.base.street, params.base.house, params.base.price,
               params.base.area, params.base.description),
      businessType(params.businessType), hasParking(params.hasParking), parkingSpaces(params.parkingSpaces),
      isVisibleFromStreet(params.isVisibleFromStreet)
{
    if (params.businessType.empty())
    {
        throw std::invalid_argument("Business type cannot be empty");
    }
    if (params.parkingSpaces < MIN_PARKING_SPACES || params.parkingSpaces > MAX_PARKING_SPACES)
    {
        throw std::invalid_argument("Parking spaces must be between 0 and 1000");
    }
}

CommercialProperty::CommercialProperty(const std::string &id, const std::string &city, const std::string &street,
                                       const std::string &house, double price, double area,
                                       const std::string &description, const std::string &businessType, bool hasParking,
                                       int parkingSpaces, bool isVisibleFromStreet)
    : CommercialProperty(CommercialPropertyParams{{id, city, street, house, price, area, description},
                                                  businessType,
                                                  hasParking,
                                                  parkingSpaces,
                                                  isVisibleFromStreet})
{
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

void CommercialProperty::setBusinessType(std::string_view type)
{
    if (type.empty())
    {
        throw std::invalid_argument("Business type cannot be empty");
    }
    businessType = std::string(type);
}

void CommercialProperty::setParkingSpaces(int spaces)
{
    if (spaces < MIN_PARKING_SPACES || spaces > MAX_PARKING_SPACES)
    {
        throw std::invalid_argument("Parking spaces must be between 0 and 1000");
    }
    parkingSpaces = spaces;
}

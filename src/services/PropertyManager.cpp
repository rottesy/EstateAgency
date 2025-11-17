#include "../../include/services/PropertyManager.h"
#include "../../include/entities/Apartment.h"
#include "../../include/entities/CommercialProperty.h"
#include "../../include/entities/House.h"
#include <algorithm>
#include <cctype>

PropertyManager::PropertyManager() = default;

void PropertyManager::addProperty(std::unique_ptr<Property> property)
{
    if (!property)
    {
        throw PropertyManagerException("Cannot add null property");
    }
    if (findProperty(property->getId()) != nullptr)
    {
        throw PropertyManagerException("Property with ID " + property->getId() + " already exists");
    }
    properties.push_back(std::move(property));
}

void PropertyManager::addApartment(const std::string &id, const std::string &city, const std::string &street,
                                   const std::string &house, double price, double area, const std::string &description,
                                   int rooms, int floor, bool hasBalcony, bool hasElevator)
{
    try
    {
        auto apartment = std::make_unique<Apartment>(id, city, street, house, price, area, description, rooms, floor,
                                                     hasBalcony, hasElevator);
        addProperty(std::move(apartment));
    }
    catch (const std::exception &e)
    {
        throw PropertyManagerException(std::string("Failed to add apartment: ") + e.what());
    }
}

void PropertyManager::addHouse(const std::string &id, const std::string &city, const std::string &street,
                               const std::string &house, double price, double area, const std::string &description,
                               int floors, int rooms, double landArea, bool hasGarage, bool hasGarden)
{
    try
    {
        auto houseObj = std::make_unique<House>(id, city, street, house, price, area, description, floors, rooms,
                                                landArea, hasGarage, hasGarden);
        addProperty(std::move(houseObj));
    }
    catch (const std::exception &e)
    {
        throw PropertyManagerException(std::string("Failed to add house: ") + e.what());
    }
}

void PropertyManager::addCommercialProperty(const std::string &id, const std::string &city, const std::string &street,
                                            const std::string &house, double price, double area,
                                            const std::string &description, const std::string &businessType,
                                            bool hasParking, int parkingSpaces, bool isVisibleFromStreet)
{
    try
    {
        auto commercial =
            std::make_unique<CommercialProperty>(id, city, street, house, price, area, description, businessType,
                                                 hasParking, parkingSpaces, isVisibleFromStreet);
        addProperty(std::move(commercial));
    }
    catch (const std::exception &e)
    {
        throw PropertyManagerException(std::string("Failed to add commercial property: ") + e.what());
    }
}

bool PropertyManager::removeProperty(const std::string &id)
{
    auto it = std::remove_if(properties.begin(), properties.end(),
                             [&id](const std::unique_ptr<Property> &prop) { return prop->getId() == id; });

    if (it != properties.end())
    {
        properties.erase(it, properties.end());
        return true;
    }
    return false;
}

Property *PropertyManager::findProperty(const std::string &id) const
{
    auto it = std::find_if(properties.begin(), properties.end(),
                           [&id](const std::unique_ptr<Property> &prop) { return prop->getId() == id; });

    return (it != properties.end()) ? it->get() : nullptr;
}

std::vector<Property *> PropertyManager::getAllProperties() const
{
    std::vector<Property *> result;
    result.reserve(properties.size());
    for (const auto &prop : properties)
    {
        result.push_back(prop.get());
    }
    return result;
}

std::vector<Property *> PropertyManager::getAvailableProperties() const
{
    std::vector<Property *> result;
    for (const auto &prop : properties)
    {
        if (prop->getIsAvailable())
        {
            result.push_back(prop.get());
        }
    }
    return result;
}

std::vector<Property *> PropertyManager::searchByPriceRange(double minPrice, double maxPrice) const
{
    std::vector<Property *> result;
    for (const auto &prop : properties)
    {
        double price = prop->getPrice();
        if (price >= minPrice && price <= maxPrice)
        {
            result.push_back(prop.get());
        }
    }
    return result;
}

std::vector<Property *> PropertyManager::searchByAddress(const std::string &city, const std::string &street,
                                                         const std::string &house) const
{
    std::vector<Property *> result;

    std::string lowerCity = city;
    std::string lowerStreet = street;
    std::string lowerHouse = house;
    std::transform(lowerCity.begin(), lowerCity.end(), lowerCity.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::transform(lowerStreet.begin(), lowerStreet.end(), lowerStreet.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::transform(lowerHouse.begin(), lowerHouse.end(), lowerHouse.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    for (const auto &prop : properties)
    {
        std::string propCity = prop->getCity();
        std::string propStreet = prop->getStreet();
        std::string propHouse = prop->getHouse();

        std::transform(propCity.begin(), propCity.end(), propCity.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::transform(propStreet.begin(), propStreet.end(), propStreet.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::transform(propHouse.begin(), propHouse.end(), propHouse.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        bool matches = true;
        if (!lowerCity.empty() && propCity.find(lowerCity) == std::string::npos)
        {
            matches = false;
        }
        if (!lowerStreet.empty() && propStreet.find(lowerStreet) == std::string::npos)
        {
            matches = false;
        }
        if (!lowerHouse.empty() && propHouse.find(lowerHouse) == std::string::npos)
        {
            matches = false;
        }

        if (matches && (!lowerCity.empty() || !lowerStreet.empty() || !lowerHouse.empty()))
        {
            result.push_back(prop.get());
        }
    }
    return result;
}

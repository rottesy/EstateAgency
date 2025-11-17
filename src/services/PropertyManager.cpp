#include "../../include/services/PropertyManager.h"
#include "../../include/core/Utils.h"
#include "../../include/entities/Apartment.h"
#include "../../include/entities/CommercialProperty.h"
#include "../../include/entities/House.h"
#include <algorithm>
#include <cctype>
#include <ranges>

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
    if (auto removed = std::ranges::remove_if(properties, [&id](const std::unique_ptr<Property> &prop)
                                              { return prop->getId() == id; });
        removed.begin() != properties.end())
    {
        properties.erase(removed.begin(), properties.end());
        return true;
    }
    return false;
}

Property *PropertyManager::findProperty(const std::string &id) const
{
    if (auto it = std::ranges::find_if(properties,
                                       [&id](const std::unique_ptr<Property> &prop) { return prop->getId() == id; });
        it != properties.end())
    {
        return it->get();
    }
    return nullptr;
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
    std::ranges::transform(lowerCity, lowerCity.begin(), [](unsigned char c) { return std::tolower(c); });
    std::ranges::transform(lowerStreet, lowerStreet.begin(), [](unsigned char c) { return std::tolower(c); });
    std::ranges::transform(lowerHouse, lowerHouse.begin(), [](unsigned char c) { return std::tolower(c); });

    for (const auto &prop : properties)
    {
        std::string propCity = prop->getCity();
        std::string propStreet = prop->getStreet();
        std::string propHouse = prop->getHouse();

        std::ranges::transform(propCity, propCity.begin(), [](unsigned char c) { return std::tolower(c); });
        std::ranges::transform(propStreet, propStreet.begin(), [](unsigned char c) { return std::tolower(c); });
        std::ranges::transform(propHouse, propHouse.begin(), [](unsigned char c) { return std::tolower(c); });

        bool matches = true;
        if (!lowerCity.empty() && !Utils::stringContains(propCity, lowerCity))
        {
            matches = false;
        }
        if (!lowerStreet.empty() && !Utils::stringContains(propStreet, lowerStreet))
        {
            matches = false;
        }
        if (!lowerHouse.empty() && !Utils::stringContains(propHouse, lowerHouse))
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

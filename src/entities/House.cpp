#include "../../include/entities/House.h"
#include <sstream>
#include <stdexcept>

namespace
{
constexpr int MIN_FLOORS = 1;
constexpr int MAX_FLOORS = 10;
constexpr int MIN_ROOMS = 1;
constexpr int MAX_ROOMS = 50;
constexpr double MIN_LAND_AREA = 0.0;
constexpr double MAX_LAND_AREA = 10000.0;
} // namespace

House::House(const std::string &id, const std::string &city, const std::string &street, const std::string &house,
             double price, double area, const std::string &description, int floors, int rooms, double landArea,
             bool hasGarage, bool hasGarden)
    : Property(id, city, street, house, price, area, description), floors(floors), rooms(rooms), landArea(landArea),
      hasGarage(hasGarage), hasGarden(hasGarden)
{
    if (floors < MIN_FLOORS || floors > MAX_FLOORS)
    {
        throw std::invalid_argument("Number of floors must be between 1 and 10");
    }
    if (rooms < MIN_ROOMS || rooms > MAX_ROOMS)
    {
        throw std::invalid_argument("Number of rooms must be between 1 and 50");
    }
    if (landArea < MIN_LAND_AREA || landArea > MAX_LAND_AREA)
    {
        throw std::invalid_argument("Land area must be between 0 and 10000 m²");
    }
}

std::string House::getType() const { return "House"; }

void House::displayInfo() const
{
    std::cout << *this << "\n";
    std::cout << "Floors: " << floors << "\n";
    std::cout << "Rooms: " << rooms << "\n";
    std::cout << "Land Area: " << landArea << " м²\n";
    std::cout << "Garage: " << (hasGarage ? "Yes" : "No") << "\n";
    std::cout << "Garden: " << (hasGarden ? "Yes" : "No") << "\n";
}

std::string House::toFileString() const
{
    std::ostringstream oss;
    oss << "HOUSE|" << id << "|" << city << "|" << street << "|" << house << "|" << price << "|" << area << "|"
        << description << "|" << (isAvailable ? "1" : "0") << "|" << floors << "|" << rooms << "|" << landArea << "|"
        << (hasGarage ? "1" : "0") << "|" << (hasGarden ? "1" : "0");
    return oss.str();
}

Property *House::clone() const
{
    return new House(id, city, street, house, price, area, description, floors, rooms, landArea, hasGarage, hasGarden);
}

void House::setFloors(int floors)
{
    if (floors < MIN_FLOORS || floors > MAX_FLOORS)
    {
        throw std::invalid_argument("Number of floors must be between 1 and 10");
    }
    this->floors = floors;
}

void House::setRooms(int rooms)
{
    if (rooms < MIN_ROOMS || rooms > MAX_ROOMS)
    {
        throw std::invalid_argument("Number of rooms must be between 1 and 50");
    }
    this->rooms = rooms;
}

void House::setLandArea(double landArea)
{
    if (landArea < MIN_LAND_AREA || landArea > MAX_LAND_AREA)
    {
        throw std::invalid_argument("Land area must be between 0 and 10000 m²");
    }
    this->landArea = landArea;
}


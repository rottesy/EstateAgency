#include "../../include/entities/House.h"
#include "../../include/entities/PropertyParams.h"
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
} 

House::House(const HouseParams &params)
    : Property(params.base.id, params.base.city, params.base.street, params.base.house, params.base.price,
               params.base.area, params.base.description),
      floors(params.floors), rooms(params.rooms), landArea(params.landArea), hasGarage(params.hasGarage),
      hasGarden(params.hasGarden)
{
    if (params.floors < MIN_FLOORS || params.floors > MAX_FLOORS)
    {
        throw std::invalid_argument("Number of floors must be between 1 and 10");
    }
    if (params.rooms < MIN_ROOMS || params.rooms > MAX_ROOMS)
    {
        throw std::invalid_argument("Number of rooms must be between 1 and 50");
    }
    if (params.landArea < MIN_LAND_AREA || params.landArea > MAX_LAND_AREA)
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
    oss << "HOUSE|" << getId() << "|" << getCity() << "|" << getStreet() << "|" << getHouse() << "|" << getPrice()
        << "|" << getArea() << "|" << getDescription() << "|" << (getIsAvailable() ? "1" : "0") << "|" << floors << "|"
        << rooms << "|" << landArea << "|" << (hasGarage ? "1" : "0") << "|" << (hasGarden ? "1" : "0");
    return oss.str();
}

Property *House::clone() const
{
    HouseParams params{{getId(), getCity(), getStreet(), getHouse(), getPrice(), getArea(), getDescription()},
                       floors,
                       rooms,
                       landArea,
                       hasGarage,
                       hasGarden};
    return new House(params);
}

void House::setFloors(int newFloors)
{
    if (newFloors < MIN_FLOORS || newFloors > MAX_FLOORS)
    {
        throw std::invalid_argument("Number of floors must be between 1 and 10");
    }
    floors = newFloors;
}

void House::setRooms(int newRooms)
{
    if (newRooms < MIN_ROOMS || newRooms > MAX_ROOMS)
    {
        throw std::invalid_argument("Number of rooms must be between 1 and 50");
    }
    rooms = newRooms;
}

void House::setLandArea(double newLandArea)
{
    if (newLandArea < MIN_LAND_AREA || newLandArea > MAX_LAND_AREA)
    {
        throw std::invalid_argument("Land area must be between 0 and 10000 m²");
    }
    landArea = newLandArea;
}

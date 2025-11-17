#include "../../include/entities/Apartment.h"
#include <sstream>
#include <stdexcept>

namespace
{
constexpr int MIN_ROOMS = 1;
constexpr int MAX_ROOMS = 10;
constexpr int MIN_FLOOR = 1;
constexpr int MAX_FLOOR = 100;
} // namespace

Apartment::Apartment(const std::string &id, const std::string &city, const std::string &street,
                     const std::string &house, double price, double area, const std::string &description, int rooms,
                     int floor, bool hasBalcony, bool hasElevator)
    : Property(id, city, street, house, price, area, description), rooms(rooms), floor(floor), hasBalcony(hasBalcony),
      hasElevator(hasElevator)
{
    if (rooms < MIN_ROOMS || rooms > MAX_ROOMS)
    {
        throw std::invalid_argument("Number of rooms must be between 1 and 10");
    }
    if (floor < MIN_FLOOR || floor > MAX_FLOOR)
    {
        throw std::invalid_argument("Floor must be between 1 and 100");
    }
}

std::string Apartment::getType() const { return "Apartment"; }

void Apartment::displayInfo() const
{
    std::cout << *this << "\n";
    std::cout << "Rooms: " << rooms << "\n";
    std::cout << "Floor: " << floor << "\n";
    std::cout << "Balcony: " << (hasBalcony ? "Yes" : "No") << "\n";
    std::cout << "Elevator: " << (hasElevator ? "Yes" : "No") << "\n";
}

std::string Apartment::toFileString() const
{
    std::ostringstream oss;
    oss << "APARTMENT|" << getId() << "|" << getCity() << "|" << getStreet() << "|" << getHouse() << "|" << getPrice()
        << "|" << getArea() << "|" << getDescription() << "|" << (getIsAvailable() ? "1" : "0") << "|" << rooms << "|"
        << floor << "|" << (hasBalcony ? "1" : "0") << "|" << (hasElevator ? "1" : "0");
    return oss.str();
}

Property *Apartment::clone() const
{
    return new Apartment(getId(), getCity(), getStreet(), getHouse(), getPrice(), getArea(), getDescription(), rooms,
                         floor, hasBalcony, hasElevator);
}

void Apartment::setRooms(int newRooms)
{
    if (newRooms < MIN_ROOMS || newRooms > MAX_ROOMS)
    {
        throw std::invalid_argument("Number of rooms must be between 1 and 10");
    }
    rooms = newRooms;
}

void Apartment::setFloor(int newFloor)
{
    if (newFloor < MIN_FLOOR || newFloor > MAX_FLOOR)
    {
        throw std::invalid_argument("Floor must be between 1 and 100");
    }
    floor = newFloor;
}

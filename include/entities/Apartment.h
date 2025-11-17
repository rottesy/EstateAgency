#ifndef APARTMENT_H
#define APARTMENT_H

#include "Property.h"
#include "PropertyParams.h"

class Apartment : public Property
{
  private:
    int rooms;
    int floor;
    bool hasBalcony;
    bool hasElevator;

  public:
    explicit Apartment(const ApartmentParams &params);

    std::string getType() const override;
    void displayInfo() const override;
    std::string toFileString() const override;
    Property *clone() const override;

    int getRooms() const { return rooms; }
    int getFloor() const { return floor; }
    bool getHasBalcony() const { return hasBalcony; }
    bool getHasElevator() const { return hasElevator; }

    void setRooms(int rooms);
    void setFloor(int floor);
};

#endif

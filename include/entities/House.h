#ifndef HOUSE_H
#define HOUSE_H

#include "Property.h"
#include "PropertyParams.h"

class House : public Property
{
  private:
    int floors;
    int rooms;
    double landArea;
    bool hasGarage;
    bool hasGarden;

  public:
    explicit House(const HouseParams &params);
    House(const std::string &id, const std::string &city, const std::string &street, const std::string &house,
          double price, double area, const std::string &description, int floors, int rooms, double landArea,
          bool hasGarage, bool hasGarden);

    std::string getType() const override;
    void displayInfo() const override;
    std::string toFileString() const override;
    Property *clone() const override;

    int getFloors() const { return floors; }
    int getRooms() const { return rooms; }
    double getLandArea() const { return landArea; }
    bool getHasGarage() const { return hasGarage; }
    bool getHasGarden() const { return hasGarden; }

    void setFloors(int floors);
    void setRooms(int rooms);
    void setLandArea(double landArea);
};

#endif

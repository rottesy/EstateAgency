#ifndef COMMERCIAL_PROPERTY_H
#define COMMERCIAL_PROPERTY_H

#include "Property.h"
#include "PropertyParams.h"
#include <string_view>

class CommercialProperty : public Property
{
  private:
    std::string businessType;
    bool hasParking;
    int parkingSpaces;
    bool isVisibleFromStreet;

  public:
    explicit CommercialProperty(const CommercialPropertyParams &params);

    std::string getType() const override;
    void displayInfo() const override;
    std::string toFileString() const override;
    Property *clone() const override;

    std::string getBusinessType() const { return businessType; }
    bool getHasParking() const { return hasParking; }
    int getParkingSpaces() const { return parkingSpaces; }
    bool getIsVisibleFromStreet() const { return isVisibleFromStreet; }

    void setBusinessType(std::string_view type);
    void setParkingSpaces(int spaces);
};

#endif

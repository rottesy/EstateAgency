#ifndef PROPERTY_PARAMS_H
#define PROPERTY_PARAMS_H

#include <string>

struct PropertyBaseParams
{
    std::string id;
    std::string city;
    std::string street;
    std::string house;
    double price;
    double area;
    std::string description;
};

struct ApartmentParams
{
    PropertyBaseParams base;
    int rooms;
    int floor;
    bool hasBalcony;
    bool hasElevator;
};

struct HouseParams
{
    PropertyBaseParams base;
    int floors;
    int rooms;
    double landArea;
    bool hasGarage;
    bool hasGarden;
};

struct CommercialPropertyParams
{
    PropertyBaseParams base;
    std::string businessType;
    bool hasParking;
    int parkingSpaces;
    bool isVisibleFromStreet;
};

#endif

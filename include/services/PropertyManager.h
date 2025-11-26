#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include "../entities/Property.h"
#include "../entities/PropertyParams.h"
#include <algorithm>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

class PropertyManagerException : public std::exception
{
  private:
    std::string message;

  public:
    explicit PropertyManagerException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

template <typename T> class ContainerManager
{
  private:
    std::vector<std::shared_ptr<T>> items;

  public:
    void add(std::shared_ptr<T> item) { items.push_back(item); }

    void remove(const std::string &id)
    {
        auto removed =
            std::ranges::remove_if(items, [&id](const std::shared_ptr<T> &item) { return item->getId() == id; });
        if (removed.begin() != items.end())
        {
            items.erase(removed.begin(), items.end());
        }
    }

    std::shared_ptr<T> findById(const std::string &id) const
    {
        if (auto it =
                std::ranges::find_if(items, [&id](const std::shared_ptr<T> &item) { return item->getId() == id; });
            it != items.end())
        {
            return *it;
        }
        return nullptr;
    }

    size_t size() const { return items.size(); }

    typename std::vector<std::shared_ptr<T>>::const_iterator begin() const { return items.begin(); }

    typename std::vector<std::shared_ptr<T>>::const_iterator end() const { return items.end(); }
};

class PropertyManager
{
  private:
    std::vector<std::unique_ptr<Property>> properties;

  public:
    PropertyManager();

    void addProperty(std::unique_ptr<Property> property);
    void addApartment(const ApartmentParams &params);
    void addHouse(const HouseParams &params);
    void addCommercialProperty(const CommercialPropertyParams &params);

    bool removeProperty(const std::string &id);
    Property *findProperty(const std::string &id) const;

    std::vector<Property *> getAllProperties() const;
    std::vector<Property *> getAvailableProperties() const;
    std::vector<Property *> searchByPriceRange(double minPrice, double maxPrice) const;
    std::vector<Property *> searchByAddress(const std::string &city, const std::string &street = "",
                                            const std::string &house = "") const;

    const std::vector<std::unique_ptr<Property>> &getProperties() const { return properties; }
    void setProperties(std::vector<std::unique_ptr<Property>> &&props) { properties = std::move(props); }

    size_t getCount() const { return properties.size(); }
};

#endif

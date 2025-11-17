#include "../../include/services/FileManager.h"
#include "../../include/core/Constants.h"
#include "../../include/entities/PropertyParams.h"
#include <fstream>
#include <sstream>
#include <string_view>

void FileManager::saveProperties(const PropertyManager &manager, const std::string &filename)
{
    const auto &properties = manager.getProperties();
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw FileManagerException("Cannot open file for writing: " + filename);
    }

    for (const auto &prop : properties)
    {
        file << prop->toFileString() << "\n";
    }
    file.close();
}

void FileManager::loadProperties(PropertyManager &manager, const std::string &filename)
{
    std::vector<std::unique_ptr<Property>> properties;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    properties.clear();
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream iss(line);
        std::string type;
        std::getline(iss, type, FILE_DELIMITER);

        try
        {
            if (type == "APARTMENT")
            {
                std::string id;
                std::string city;
                std::string street;
                std::string house;
                std::string desc;
                std::string avail;
                double price;
                double area;
                int rooms;
                int floor;
                int balcony;
                int elevator;

                std::getline(iss, id, FILE_DELIMITER);
                std::getline(iss, city, FILE_DELIMITER);
                std::getline(iss, street, FILE_DELIMITER);
                std::getline(iss, house, FILE_DELIMITER);
                iss >> price;
                iss.ignore();
                iss >> area;
                iss.ignore();
                std::getline(iss, desc, FILE_DELIMITER);
                std::getline(iss, avail, FILE_DELIMITER);
                iss >> rooms;
                iss.ignore();
                iss >> floor;
                iss.ignore();
                iss >> balcony;
                iss.ignore();
                iss >> elevator;

                ApartmentParams params{
                    {id, city, street, house, price, area, desc}, rooms, floor, balcony == 1, elevator == 1};
                auto apartment = std::make_unique<Apartment>(params);
                apartment->setAvailable(avail == std::string(1, AVAILABLE_CHAR));
                properties.push_back(std::move(apartment));
            }
            else if (type == "HOUSE")
            {
                std::string id;
                std::string city;
                std::string street;
                std::string house;
                std::string desc;
                std::string avail;
                double price;
                double area;
                double landArea;
                int floors;
                int rooms;
                int garage;
                int garden;

                std::getline(iss, id, FILE_DELIMITER);
                std::getline(iss, city, FILE_DELIMITER);
                std::getline(iss, street, FILE_DELIMITER);
                std::getline(iss, house, FILE_DELIMITER);
                iss >> price;
                iss.ignore();
                iss >> area;
                iss.ignore();
                std::getline(iss, desc, FILE_DELIMITER);
                std::getline(iss, avail, FILE_DELIMITER);
                iss >> floors;
                iss.ignore();
                iss >> rooms;
                iss.ignore();
                iss >> landArea;
                iss.ignore();
                iss >> garage;
                iss.ignore();
                iss >> garden;

                HouseParams params{
                    {id, city, street, house, price, area, desc}, floors, rooms, landArea, garage == 1, garden == 1};
                auto houseObj = std::make_unique<House>(params);
                houseObj->setAvailable(avail == std::string(1, AVAILABLE_CHAR));
                properties.push_back(std::move(houseObj));
            }
            else if (type == "COMMERCIAL")
            {
                std::string id;
                std::string city;
                std::string street;
                std::string house;
                std::string desc;
                std::string avail;
                std::string businessType;
                double price;
                double area;
                int parking;
                int visible;
                int parkingSpaces;

                std::getline(iss, id, FILE_DELIMITER);
                std::getline(iss, city, FILE_DELIMITER);
                std::getline(iss, street, FILE_DELIMITER);
                std::getline(iss, house, FILE_DELIMITER);
                iss >> price;
                iss.ignore();
                iss >> area;
                iss.ignore();
                std::getline(iss, desc, FILE_DELIMITER);
                std::getline(iss, avail, FILE_DELIMITER);
                std::getline(iss, businessType, FILE_DELIMITER);
                iss >> parking;
                iss.ignore();
                iss >> parkingSpaces;
                iss.ignore();
                iss >> visible;

                CommercialPropertyParams params{{id, city, street, house, price, area, desc},
                                                businessType,
                                                parking == 1,
                                                parkingSpaces,
                                                visible == 1};
                auto commercial = std::make_unique<CommercialProperty>(params);
                commercial->setAvailable(avail == std::string(1, AVAILABLE_CHAR));
                properties.push_back(std::move(commercial));
            }
        }
        catch (const std::invalid_argument &)
        {
            // Skip invalid property data
            continue;
        }
        catch (const PropertyManagerException &)
        {
            // Skip property manager errors
            continue;
        }
    }

    file.close();
    manager.setProperties(std::move(properties));
}

void FileManager::saveClients(const ClientManager &manager, const std::string &filename)
{
    const auto &clients = manager.getClients();
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw FileManagerException("Cannot open file for writing: " + filename);
    }

    for (const auto &client : clients)
    {
        file << client->getId() << FILE_DELIMITER << client->getName() << FILE_DELIMITER << client->getPhone()
             << FILE_DELIMITER << client->getEmail() << FILE_DELIMITER << client->getRegistrationDate() << "\n";
    }
    file.close();
}

void FileManager::loadClients(ClientManager &manager, const std::string &filename)
{
    std::vector<std::shared_ptr<Client>> clients;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    clients.clear();
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream iss(line);
        std::string id;
        std::string name;
        std::string phone;
        std::string email;
        std::string regDate;

        std::getline(iss, id, FILE_DELIMITER);
        std::getline(iss, name, FILE_DELIMITER);
        std::getline(iss, phone, FILE_DELIMITER);
        std::getline(iss, email, FILE_DELIMITER);
        std::getline(iss, regDate, FILE_DELIMITER);

        try
        {
            auto client = std::make_shared<Client>(id, name, phone, email);
            clients.push_back(client);
        }
        catch (const std::invalid_argument &)
        {
            // Skip invalid client data
            continue;
        }
        catch (const ClientManagerException &)
        {
            // Skip client manager errors
            continue;
        }
    }

    file.close();
    manager.setClients(std::move(clients));
}

void FileManager::saveTransactions(const TransactionManager &manager, const std::string &filename)
{
    const auto &transactions = manager.getTransactions();
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw FileManagerException("Cannot open file for writing: " + filename);
    }

    for (const auto &trans : transactions)
    {
        file << trans->toFileString() << "\n";
    }
    file.close();
}

void FileManager::loadTransactions(TransactionManager &manager, const std::string &filename)
{
    std::vector<std::shared_ptr<Transaction>> transactions;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    transactions.clear();
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream iss(line);
        std::string id;
        std::string propertyId;
        std::string clientId;
        std::string date;
        std::string status;
        std::string notes;
        double finalPrice;

        std::getline(iss, id, FILE_DELIMITER);
        std::getline(iss, propertyId, FILE_DELIMITER);
        std::getline(iss, clientId, FILE_DELIMITER);
        std::getline(iss, date, FILE_DELIMITER);
        iss >> finalPrice;
        iss.ignore();
        std::getline(iss, status, FILE_DELIMITER);
        std::getline(iss, notes, FILE_DELIMITER);

        try
        {
            auto transaction = std::make_shared<Transaction>(id, propertyId, clientId, finalPrice, status, notes);
            transactions.push_back(transaction);
        }
        catch (const std::invalid_argument &)
        {
            // Skip invalid transaction data
            continue;
        }
        catch (const TransactionManagerException &)
        {
            // Skip transaction manager errors
            continue;
        }
    }

    file.close();
    manager.setTransactions(std::move(transactions));
}

void FileManager::saveAuctions(const AuctionManager &manager, const std::string &filename)
{
    const auto &auctions = manager.getAuctions();
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw FileManagerException("Cannot open file for writing: " + filename);
    }

    for (const auto &auction : auctions)
    {
        file << auction->toFileString() << "\n";

        auto bids = auction->getBids();
        for (const auto &bid : bids)
        {
            file << "BID|" << auction->getId() << FILE_DELIMITER << bid->toFileString() << "\n";
        }
    }
    file.close();
}

void FileManager::parseBidLine(std::string_view line, Auction *currentAuction)
{
    if (currentAuction == nullptr)
    {
        return;
    }

    try
    {
        std::string lineStr(line);
        std::istringstream iss(lineStr.substr(BID_PREFIX_LENGTH));
        std::string auctionId;
        std::string clientId;
        std::string clientName;
        std::string amountStr;
        std::string timestamp;

        if (!std::getline(iss, auctionId, FILE_DELIMITER) || auctionId.empty())
        {
            return;
        }
        if (!std::getline(iss, clientId, FILE_DELIMITER) || clientId.empty())
        {
            return;
        }
        if (!std::getline(iss, clientName, FILE_DELIMITER))
        {
            return;
        }
        if (!std::getline(iss, amountStr, FILE_DELIMITER) || amountStr.empty())
        {
            return;
        }
        std::getline(iss, timestamp, FILE_DELIMITER);

        double amount = std::stod(amountStr);
        if (amount <= 0.0)
        {
            return;
        }

        auto bid = std::make_shared<Bid>(clientId, clientName, amount);
        currentAuction->addBidDirect(bid);
    }
    catch (const std::invalid_argument &e)
    {
        // Skip invalid bid data
        (void)e; // Suppress unused variable warning
    }
    catch (const AuctionManagerException &e)
    {
        // Skip auction manager errors
        (void)e; // Suppress unused variable warning
    }
}

std::shared_ptr<Auction> FileManager::parseAuctionLine(std::string_view line)
{
    std::string lineStr(line);
    std::istringstream iss(lineStr);
    std::string id;
    std::string propertyId;
    std::string propertyAddress;
    std::string startingPriceStr;
    std::string buyoutPriceStr;
    std::string status;
    std::string createdAt;
    std::string completedAt;

    std::getline(iss, id, FILE_DELIMITER);
    std::getline(iss, propertyId, FILE_DELIMITER);
    std::getline(iss, propertyAddress, FILE_DELIMITER);
    std::getline(iss, startingPriceStr, FILE_DELIMITER);
    std::getline(iss, buyoutPriceStr, FILE_DELIMITER);
    std::getline(iss, status, FILE_DELIMITER);
    std::getline(iss, createdAt, FILE_DELIMITER);
    std::getline(iss, completedAt, FILE_DELIMITER);

    try
    {
        double startingPrice = std::stod(startingPriceStr);
        auto auction = std::make_shared<Auction>(id, propertyId, propertyAddress, startingPrice);

        if (status == Constants::AuctionStatus::COMPLETED)
        {
            auction->complete();
        }
        else if (status == Constants::AuctionStatus::CANCELLED)
        {
            auction->cancel();
        }
        return auction;
    }
    catch (const std::invalid_argument &e)
    {
        // Skip invalid auction data
        (void)e; // Suppress unused variable warning
        return nullptr;
    }
    catch (const AuctionManagerException &e)
    {
        // Skip auction manager errors
        (void)e; // Suppress unused variable warning
        return nullptr;
    }
}

void FileManager::loadAuctions(AuctionManager &manager, const std::string &filename)
{
    std::vector<std::shared_ptr<Auction>> auctions;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    auctions.clear();
    std::string line;
    Auction *currentAuction = nullptr;
    const std::string BID_PREFIX = "BID|";

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        if (line.starts_with(BID_PREFIX))
        {
            parseBidLine(line, currentAuction);
        }
        else
        {
            auto auction = parseAuctionLine(line);
            if (auction != nullptr)
            {
                auctions.push_back(auction);
                currentAuction = auction.get();
            }
        }
    }

    file.close();
    manager.setAuctions(std::move(auctions));
}

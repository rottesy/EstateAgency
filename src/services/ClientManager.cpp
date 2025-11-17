#include "../../include/services/ClientManager.h"
#include "../../include/core/Utils.h"
#include <algorithm>
#include <cctype>

ClientManager::ClientManager() = default;

void ClientManager::addClient(std::shared_ptr<Client> client)
{
    if (!client)
    {
        throw ClientManagerException("Cannot add null client");
    }
    if (findClient(client->getId()) != nullptr)
    {
        throw ClientManagerException("Client with ID " + client->getId() + " already exists");
    }
    clients.push_back(client);
}

bool ClientManager::removeClient(const std::string &id)
{
    auto it = std::remove_if(clients.begin(), clients.end(),
                             [&id](const std::shared_ptr<Client> &client) { return client->getId() == id; });

    if (it != clients.end())
    {
        clients.erase(it, clients.end());
        return true;
    }
    return false;
}

Client *ClientManager::findClient(const std::string &id) const
{
    auto it = std::find_if(clients.begin(), clients.end(),
                           [&id](const std::shared_ptr<Client> &client) { return client->getId() == id; });

    return (it != clients.end()) ? it->get() : nullptr;
}

std::vector<Client *> ClientManager::getAllClients() const
{
    std::vector<Client *> result;
    result.reserve(clients.size());
    for (const auto &client : clients)
    {
        result.push_back(client.get());
    }
    return result;
}

std::vector<Client *> ClientManager::searchByName(const std::string &name) const
{
    std::vector<Client *> result;
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    for (const auto &client : clients)
    {
        std::string clientName = client->getName();
        std::transform(clientName.begin(), clientName.end(), clientName.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (Utils::stringContains(clientName, lowerName))
        {
            result.push_back(client.get());
        }
    }
    return result;
}

std::vector<Client *> ClientManager::searchByPhone(const std::string &phone) const
{
    std::vector<Client *> result;
    for (const auto &client : clients)
    {
        if (client->getPhone() == phone)
        {
            result.push_back(client.get());
        }
    }
    return result;
}

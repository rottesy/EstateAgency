#include "../../include/services/ClientManager.h"
#include "../../include/core/Utils.h"
#include <algorithm>
#include <cctype>
#include <ranges>
#include <string_view>

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
    auto removed =
        std::ranges::remove_if(clients, [&id](const std::shared_ptr<Client> &client) { return client->getId() == id; });
    if (removed.begin() != clients.end())
    {
        clients.erase(removed.begin(), clients.end());
        return true;
    }
    return false;
}

Client *ClientManager::findClient(const std::string &id) const
{
    if (auto it = std::ranges::find_if(clients,
                                       [&id](const std::shared_ptr<Client> &client) { return client->getId() == id; });
        it != clients.end())
    {
        return it->get();
    }
    return nullptr;
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

std::vector<Client *> ClientManager::searchByName(std::string_view name) const
{
    std::vector<Client *> result;
    std::string lowerName(name.data(), name.size());
    std::ranges::transform(lowerName, lowerName.begin(), [](unsigned char c) { return std::tolower(c); });

    for (const auto &client : clients)
    {
        std::string clientName = client->getName();
        std::ranges::transform(clientName, clientName.begin(), [](unsigned char c) { return std::tolower(c); });
        if (Utils::stringContains(clientName, lowerName))
        {
            result.push_back(client.get());
        }
    }
    return result;
}

std::vector<Client *> ClientManager::searchByPhone(std::string_view phone) const
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

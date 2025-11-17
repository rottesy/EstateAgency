#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "../entities/Client.h"
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class ClientManagerException : public std::exception
{
  private:
    std::string message;

  public:
    explicit ClientManagerException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

class ClientManager
{
  private:
    std::vector<std::shared_ptr<Client>> clients;

  public:
    ClientManager();

    void addClient(std::shared_ptr<Client> client);
    bool removeClient(const std::string &id);
    Client *findClient(const std::string &id) const;

    std::vector<Client *> getAllClients() const;
    std::vector<Client *> searchByName(std::string_view name) const;
    std::vector<Client *> searchByPhone(std::string_view phone) const;

    // Методы для доступа к данным для FileManager
    const std::vector<std::shared_ptr<Client>> &getClients() const { return clients; }
    void setClients(std::vector<std::shared_ptr<Client>> &&newClients) { clients = std::move(newClients); }

    size_t getCount() const { return clients.size(); }
};

#endif

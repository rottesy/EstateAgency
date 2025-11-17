#ifndef CLIENT_H
#define CLIENT_H

#include <compare>
#include <iostream>
#include <string>
#include <string_view>

class Client
{
  private:
    std::string id;
    std::string name;
    std::string phone;
    std::string email;
    std::string registrationDate;

  public:
    Client(const std::string &id, const std::string &name, const std::string &phone, const std::string &email);

    friend std::ostream &operator<<(std::ostream &os, const Client &client);

    bool operator==(const Client &other) const;
    std::strong_ordering operator<=>(const Client &other) const;

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }
    std::string getRegistrationDate() const { return registrationDate; }

    void setName(const std::string &name);
    void setPhone(const std::string &phone);
    void setEmail(const std::string &email);

    static bool validateId(std::string_view id);
    static bool validatePhone(std::string_view phone);
    static bool validateEmail(std::string_view email);

    std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os, const Client &client)
    {
        os << "ID: " << client.id << "\n"
           << "Name: " << client.name << "\n"
           << "Phone: " << client.phone << "\n"
           << "Email: " << client.email << "\n"
           << "Registration Date: " << client.registrationDate;
        return os;
    }
};

#endif

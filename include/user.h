#ifndef USER_H
#define USER_H

#include <string>
#include <functional>
#include <fstream>

class User {
protected:
    int id;
    std::string username;
    size_t passwordHash;   // hashed password
    std::string role;
    std::string email;

    static size_t hashPassword(const std::string& password) {
        return std::hash<std::string>{}(password);
    }

public:
    User(int id = 0,
         std::string username = "",
         std::string password = "",
         std::string role = "",
         std::string email = "")
        : id(id), username(username), role(role), email(email) {
        passwordHash = hashPassword(password);
    }

    virtual ~User() {}

    int getID() const { return id; }
    std::string getUsername() const { return username; }
    std::string getEmail() const { return email; }
    std::string getRole() const { return role; }

    bool authenticate(const std::string& inputPassword) const {
        return passwordHash == hashPassword(inputPassword);
    }

    virtual void displayMenu() = 0;

    virtual void saveToFile(std::ofstream& out) const {
        out << id << " " << username << " " << passwordHash << " "
            << role << " " << email << "\n";
    }

    virtual void loadFromFile(std::ifstream& in) {
        in >> id >> username >> passwordHash >> role >> email;
    }
};

#endif
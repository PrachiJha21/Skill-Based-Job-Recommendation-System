#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"
#include <unordered_map>
#include <unordered_set>
#include <string>

class Job;
class Candidate;
class Employer;

class Admin : public User {
public:
    // Hardcoded admin credentials
    static const std::string ADMIN_EMAIL;
    static const std::string ADMIN_PASSWORD;
    static const std::string ADMIN_USERNAME;

    Admin(int id = 0,
          std::string username = "",
          std::string password = "",
          std::string email = "");

    // Static validation method for admin authentication
    static bool validateAdminCredentials(const std::string& email, const std::string& password);

    // Menu
    void displayMenu() override;

    // Admin actions
    void approveJob(int jobID, std::unordered_set<int>& approvedJobs);

    void removeUser(const std::string& email,
                    std::unordered_map<std::string, Candidate*>& candidates,
                    std::unordered_map<std::string, Employer*>& employers);

    // Reports
    void generateSkillDemandReport(
        const std::unordered_map<int, Job*>& jobs,
        const std::unordered_set<int>& approvedJobs
    );

    void systemStatistics(
        const std::unordered_map<std::string, Candidate*>& candidates,
        const std::unordered_map<std::string, Employer*>& employers,
        const std::unordered_map<int, Job*>& jobs,
        const std::unordered_set<int>& approvedJobs
    );

    // File handling
    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;
};

#endif

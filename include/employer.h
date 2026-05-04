#ifndef EMPLOYER_H
#define EMPLOYER_H

#include "user.h"
#include <vector>
#include <string>
#include <fstream>

class Employer : public User {
private:
    std::string companyName;
    std::vector<int> postedJobIDs;

public:
    Employer(
        int id = 0,
        std::string username = "",
        std::string password = "",
        std::string email = "",
        std::string companyName = ""
    );

    /* ===== Job Management ===== */
    void postJob();
    void editJob(int jobId);
    void deleteJob(int jobId);
    void viewApplicants(int jobId) const;

    /* ===== Getters ===== */
    std::string getCompanyName() const;
    const std::vector<int>& getPostedJobIDs() const;

    /* ===== Menu ===== */
    void displayMenu() override;

    /* ===== CSV Persistence ===== */
    std::string toCSV() const;
    void fromCSV(const std::string& csvLine);

    /* ===== File Handling ===== */
    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;
};

#endif
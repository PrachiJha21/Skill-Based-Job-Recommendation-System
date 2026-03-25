#include "employer.h"
#include <iostream>
#include <algorithm>

/* =========================
   Constructor
   ========================= */

Employer::Employer(
    int id,
    std::string username,
    std::string password,
    std::string email,
    std::string companyName
)
    : User(id, username, password, "employer", email),
      companyName(companyName) {}

/* =========================
   Job Management
   ========================= */

void Employer::postJob() {
    std::cout << "Posting a new job (handled by MatchingEngine).\n";
    // MatchingEngine will generate job ID and store it
}

void Employer::editJob(int jobId) {
    std::cout << "Editing job with ID: " << jobId << "\n";
    // MatchingEngine will validate and modify the job
}

void Employer::deleteJob(int jobId) {
    auto it = std::find(postedJobIDs.begin(),
                        postedJobIDs.end(),
                        jobId);

    if (it != postedJobIDs.end()) {
        postedJobIDs.erase(it);
        std::cout << "Deleted job with ID: " << jobId << "\n";
    } else {
        std::cout << "Job ID not found.\n";
    }
}

void Employer::viewApplicants(int jobId) const {
    std::cout << "Viewing applicants for job ID: "
              << jobId
              << " (handled by MatchingEngine).\n";
}

/* =========================
   Getters
   ========================= */

std::string Employer::getCompanyName() const {
    return companyName;
}

const std::vector<int>& Employer::getPostedJobIDs() const {
    return postedJobIDs;
}

/* =========================
   Menu
   ========================= */

void Employer::displayMenu() {
    int choice;

    do {
        std::cout << "\n======== Employer Menu ========\n";
        std::cout << "1. Post Job\n";
        std::cout << "2. Edit Job\n";
        std::cout << "3. Delete Job\n";
        std::cout << "4. View Applicants\n";
        std::cout << "5. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            postJob();
            break;

        case 2: {
            int jobId;
            std::cout << "Enter Job ID to edit: ";
            std::cin >> jobId;
            editJob(jobId);
            break;
        }

        case 3: {
            int jobId;
            std::cout << "Enter Job ID to delete: ";
            std::cin >> jobId;
            deleteJob(jobId);
            break;
        }

        case 4: {
            int jobId;
            std::cout << "Enter Job ID to view applicants: ";
            std::cin >> jobId;
            viewApplicants(jobId);
            break;
        }

        case 5:
            std::cout << "Logging out...\n";
            break;

        default:
            std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);   // ✅ stay in Employer menu until logout
}

/* =========================
   File Handling
   ========================= */

void Employer::saveToFile(std::ofstream& out) const {
    // Save base User data
    User::saveToFile(out);

    // Save company name
    out << companyName << "\n";

    // Save posted job IDs
    out << postedJobIDs.size() << "\n";
    for (int jobId : postedJobIDs) {
        out << jobId << " ";
    }
    out << "\n";
}

void Employer::loadFromFile(std::ifstream& in) {
    // Load base User data
    User::loadFromFile(in);

    // Load company name
    in >> companyName;

    // Load posted job IDs
    size_t jobCount;
    in >> jobCount;
    postedJobIDs.clear();
    for (size_t i = 0; i < jobCount; ++i) {
        int jobId;
        in >> jobId;
        postedJobIDs.push_back(jobId);
    }
}

#include "admin.h"
#include "job.h"
#include "candidate.h"
#include "employer.h"
#include "systemManager.h"
#include <iostream>
#include <iomanip>
#include "utils.h"

using namespace std;

// Static Admin Credentials

const std::string Admin::ADMIN_EMAIL = "admin@jobrecommendation.com";
const std::string Admin::ADMIN_PASSWORD = "Admin@Secure123";
const std::string Admin::ADMIN_USERNAME = "Administrator";


// Admin Authentication

bool Admin::validateAdminCredentials(const std::string& email, const std::string& password) {
    return (email == ADMIN_EMAIL && password == ADMIN_PASSWORD);
}

// Constructor
Admin::Admin(int id,
             string username,
             string password,
             string email)
    : User(id, username, password, "admin", email) {}



// Admin Menu

void Admin::displayMenu() {
    int choice;

    do {
        printLine('=');
        SetConsoleTextAttribute(h, 3);
        centerText("Admin Dashboard");
        SetConsoleTextAttribute(h, 7);
        printLine('=');
        SetConsoleTextAttribute(h, 5);
        cout << "1. Remove Job (Scam Report)\n";
        cout << "2. Remove User\n";
        cout << "3. System Statistics\n";
        cout << "4. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        SetConsoleTextAttribute(h, 7);

        switch (choice) {
            case 1: {
                int jobId;
                printLine('.');
                SetConsoleTextAttribute(h, 12);
                centerText("Enter Job ID to remove (scam report): ");
                cin >> jobId;
                removeJob(jobId);
                SetConsoleTextAttribute(h, 7);
                printLine('.');
                break;
            }
            case 2: {
                string email;
                printLine('.');
                SetConsoleTextAttribute(h, 12);
                centerText("Enter email to remove: ");
                cin >> email;
                removeUser(
                    email,
                    SystemManager::getInstance().getCandidates(),
                    SystemManager::getInstance().getEmployers()
                );
                SetConsoleTextAttribute(h, 7);
                printLine('.');
                break;
            }

            case 3:
                printLine('.');
                systemStatistics(
                    SystemManager::getInstance().getCandidates(),
                    SystemManager::getInstance().getEmployers(),
                    SystemManager::getInstance().getAllJobs(),
                    SystemManager::getInstance().getApprovedJobs()
                );
                printLine('.');
                break;

            case 4:
                SetConsoleTextAttribute(h, 12);
                centerText("Logging out...");
                SetConsoleTextAttribute(h, 7);
                break;

            default:
                SetConsoleTextAttribute(h, 12);
                centerText("Invalid choice. Try again.");
                SetConsoleTextAttribute(h, 7);
            }

        } while (choice != 4);   // ✅ stay in admin session until logout
}



// Remove Job (Scam Report)

void Admin::removeJob(int jobID) {
    if (SystemManager::getInstance().removeJob(jobID)) {
        SetConsoleTextAttribute(h, 10);
        centerText("Job removed successfully due to scam report.");
        SetConsoleTextAttribute(h, 7);
        SystemManager::getInstance().saveData();  // Auto-save after job removal
    } else {
        SetConsoleTextAttribute(h, 12);
        centerText("Job not found.");
        SetConsoleTextAttribute(h, 7);
    }
}


// ============================
// Remove User
// ============================
void Admin::removeUser(const string& email,
                       unordered_map<string, Candidate*>& candidates,
                       unordered_map<string, Employer*>& employers) {

    auto cit = candidates.find(email);
    if (cit != candidates.end()) {
        delete cit->second;
        candidates.erase(cit);
        SetConsoleTextAttribute(h, 10);
        centerText("Candidate removed successfully.");
        SetConsoleTextAttribute(h, 7);
        SystemManager::getInstance().saveData();  // Auto-save after user removal
        return;
    }

    auto eit = employers.find(email);
    if (eit != employers.end()) {
        delete eit->second;
        employers.erase(eit);
        SetConsoleTextAttribute(h, 10);
        centerText("Employer removed successfully.");
        SetConsoleTextAttribute(h, 7);
        SystemManager::getInstance().saveData();  // Auto-save after user removal
        return;
    }

    SetConsoleTextAttribute(h, 12);
    centerText("User not found.");
    SetConsoleTextAttribute(h, 7);
}


// ============================
// System Statistics
// ============================
void Admin::systemStatistics(
        const unordered_map<string, Candidate*>& candidates,
        const unordered_map<string, Employer*>& employers,
        const unordered_map<int, Job*>& jobs,
        const unordered_set<int>& approvedJobs) {

    SetConsoleTextAttribute(h, 14);
    centerText("SYSTEM STATISTICS");
    SetConsoleTextAttribute(h, 7);

    cout << "Candidates      : " << candidates.size() << endl;
    cout << "Employers       : " << employers.size() << endl;
    cout << "Total Jobs      : " << jobs.size() << endl;
    cout << "Approved Jobs   : " << approvedJobs.size() << endl;
}


// ============================
// File Handling
// ============================
void Admin::saveToFile(ofstream& out) const {
    User::saveToFile(out);
}

void Admin::loadFromFile(ifstream& in) {
    User::loadFromFile(in);
}

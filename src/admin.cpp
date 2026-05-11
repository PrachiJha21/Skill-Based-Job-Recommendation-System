#include "admin.h"
#include "job.h"
#include "candidate.h"
#include "employer.h"
#include "systemManager.h"
#include <iostream>
#include <iomanip>

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
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');   // reset fill character
        cout << setw(42) <<" " << "Admin Dashboard" << endl;
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');
        cout << "1. Remove Job (Scam Report)\n";
        cout << "2. Remove User\n";
        cout << "3. Skill Demand Report\n";
        cout << "4. System Statistics\n";
        cout << "5. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int jobId;
                cout << "Enter Job ID to remove (scam report): ";
                cin >> jobId;
                removeJob(jobId);
                break;
            }
            case 2: {
                string email;
                cout << "Enter email to remove: ";
                cin >> email;
                removeUser(
                    email,
                    SystemManager::getInstance().getCandidates(),
                    SystemManager::getInstance().getEmployers()
                );
                break;
            }

            case 3:
                generateSkillDemandReport(
                    SystemManager::getInstance().getAllJobs(),
                    SystemManager::getInstance().getApprovedJobs()
                );
                break;

            case 4:
                systemStatistics(
                    SystemManager::getInstance().getCandidates(),
                    SystemManager::getInstance().getEmployers(),
                    SystemManager::getInstance().getAllJobs(),
                    SystemManager::getInstance().getApprovedJobs()
                );
                break;

            case 5:
                cout << "Logging out...\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
            }

        } while (choice != 5);   // ✅ stay in admin session until logout
}



// Remove Job (Scam Report)

void Admin::removeJob(int jobID) {
    if (SystemManager::getInstance().removeJob(jobID)) {
        cout << "Job removed successfully due to scam report.\n";
        SystemManager::getInstance().saveData();  // Auto-save after job removal
    } else {
        cout << "Job not found.\n";
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
        cout << "Candidate removed successfully.\n";
        SystemManager::getInstance().saveData();  // Auto-save after user removal
        return;
    }

    auto eit = employers.find(email);
    if (eit != employers.end()) {
        delete eit->second;
        employers.erase(eit);
        cout << "Employer removed successfully.\n";
        SystemManager::getInstance().saveData();  // Auto-save after user removal
        return;
    }

    cout << "User not found.\n";
}


// ============================
// Skill Demand Report
// ============================
void Admin::generateSkillDemandReport(
        const unordered_map<int, Job*>& jobs,
        const unordered_set<int>& approvedJobs) {

    unordered_map<string, int> skillCount;

    for (int jobID : approvedJobs) {
        auto it = jobs.find(jobID);
        if (it == jobs.end() || it->second == nullptr)
            continue;

        const Job* job = it->second;
        for (const string& skill : job->getRequiredSkills()) {
            skillCount[skill]++;
        }
    }

    cout << "\n===== SKILL DEMAND REPORT =====\n";
    for (const auto& entry : skillCount) {
        cout << entry.first << " : " << entry.second << endl;
    }
}


// ============================
// System Statistics
// ============================
void Admin::systemStatistics(
        const unordered_map<string, Candidate*>& candidates,
        const unordered_map<string, Employer*>& employers,
        const unordered_map<int, Job*>& jobs,
        const unordered_set<int>& approvedJobs) {

    cout << "\n===== SYSTEM STATISTICS =====\n";
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

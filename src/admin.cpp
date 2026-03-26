#include "admin.h"
#include "job.h"
#include "candidate.h"
#include "employer.h"
#include "systemManager.h"
#include <iostream>

using namespace std;

// Constructor
Admin::Admin(int id,
             string username,
             string password,
             string email)
    : User(id, username, password, "admin", email) {}


// ============================
// Admin Menu
// ============================
void Admin::displayMenu() {
    int choice;

    do {
        cout << "\n======== ADMIN MENU ========\n";
        cout << "1. Approve Job\n";
        cout << "2. Remove User\n";
        cout << "3. Skill Demand Report\n";
        cout << "4. System Statistics\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int jobId;
            cout << "Enter Job ID to approve: ";
            cin >> jobId;
            approveJob(jobId, SystemManager::getInstance().getApprovedJobs());
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

        case 0:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);   // ✅ stay in admin session until logout
}


// ============================
// Approve Job
// ============================
void Admin::approveJob(int jobID,
                       unordered_set<int>& approvedJobs) {

    if (approvedJobs.count(jobID)) {
        cout << "Job is already approved.\n";
        return;
    }

    approvedJobs.insert(jobID);
    cout << "Job approved successfully.\n";
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
        return;
    }

    auto eit = employers.find(email);
    if (eit != employers.end()) {
        delete eit->second;
        employers.erase(eit);
        cout << "Employer removed successfully.\n";
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

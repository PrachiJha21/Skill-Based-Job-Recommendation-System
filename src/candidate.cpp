#include "candidate.h"
#include "systemManager.h"
#include "matchingengine.h"
#include "job.h"

#include <iostream>

using namespace std;

/* =========================================================
   Constructor
   ========================================================= */

Candidate::Candidate(
    int id,
    string username,
    string password,
    string email
)
    : User(id, username, password, "candidate", email),
      profileStrengthScore(0.0) {}

/* =========================================================
   Skill Management
   ========================================================= */

void Candidate::addSkill(const string& skill) {
    skills.insert(skill);
    updateProfileStrength();
    cout << "Skill added successfully.\n";
}

void Candidate::removeSkill(const string& skill) {
    if (skills.erase(skill)) {
        updateProfileStrength();
        cout << "Skill removed successfully.\n";
    } else {
        cout << "Skill not found.\n";
    }
}

const unordered_set<string>& Candidate::getSkills() const {
    return skills;
}

/* =========================================================
   Profile Strength
   ========================================================= */

double Candidate::calculateMatch(int /*jobId*/) const {
    //Placeholder: MatchingEngine should supplu real ranked Jobs
    return {};
}
void Candidate::updateProfileStrength() {
    profileStrengthScore = static_cast<double>(skills.size()) * 10.0;
}

double Candidate::getProfileStrength() const {
    return profileStrengthScore;
}

/* =========================================================
   Job Application
   ========================================================= */

void Candidate::applyForJob(int jobId) {
    SystemManager::getInstance().submitApplication(
        getID(), jobId, 0.0
    );
    appliedJobIDs.push_back(jobId);
}

/* =========================================================
   Skill Gap
   ========================================================= */

void Candidate::viewSkillGap(int jobId) const {
    auto gaps = SystemManager::getInstance().getSkillGap(*this, jobId);

    if (gaps.empty()) {
        cout << "No skill gap. You match this job well.\n";
        return;
    }

    cout << "Missing skills:\n";
    for (const string& skill : gaps) {
        cout << "- " << skill << endl;
    }
}

/* =========================================================
   Candidate Menu (SESSION LOOP)
   ========================================================= */

void Candidate::displayMenu() {
    int choice;

    do {
        cout << "\n======== Candidate Menu ========\n";
        cout << "1. Add Skill\n";
        cout << "2. Remove Skill\n";
        cout << "3. View Profile Strength\n";
        cout << "4. View Top Matching Jobs\n";
        cout << "5. View Skill Gap for Job\n";
        cout << "6. Apply for Job\n";
        cout << "7. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string skill;
            cout << "Enter skill: ";
            cin >> skill;
            addSkill(skill);
            break;
        }

        case 2: {
            string skill;
            cout << "Enter skill to remove: ";
            cin >> skill;
            removeSkill(skill);
            break;
        }

        case 3:
            cout << "Profile Strength: "
                 << getProfileStrength() << "%\n";
            break;

        case 4: {
            auto matches =
                SystemManager::getInstance().getTopMatchingJobs(*this);

            if (matches.empty()) {
                cout << "No matching jobs found.\n";
            } else {
                cout << "Top Matching Job IDs:\n";
                for (int id : matches) {
                    cout << "- Job ID: " << id << endl;
                }
            }
            break;
        }

        case 5: {
            int jobId;
            cout << "Enter Job ID: ";
            cin >> jobId;
            viewSkillGap(jobId);
            break;
        }

        case 6: {
            int jobId;
            cout << "Enter Job ID to apply: ";
            cin >> jobId;
            applyForJob(jobId);
            break;
        }

        case 7:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 7);  // ✅ SESSION ENDS ONLY ON LOGOUT
}

/* =========================================================
   File Handling
   ========================================================= */

void Candidate::saveToFile(ofstream& out) const {
    User::saveToFile(out);

    out << skills.size() << endl;
    for (const auto& skill : skills) {
        out << skill << endl;
    }
}

void Candidate::loadFromFile(ifstream& in) {
    User::loadFromFile(in);

    size_t count;
    in >> count;

    skills.clear();
    for (size_t i = 0; i < count; i++) {
        string skill;
        in >> skill;
        skills.insert(skill);
    }

    updateProfileStrength();
}

#include "systemManager.h"

#include "user.h"
#include "candidate.h"
#include "employer.h"
#include "admin.h"
#include "job.h"
#include "application.h"
#include "matchingengine.h"

#include <iostream>

using namespace std;

/* =========================================================
   Singleton Implementation
   ========================================================= */

SystemManager::SystemManager()
    : nextUserId(1),
      nextJobId(1),
      nextApplicationId(1) {}

SystemManager& SystemManager::getInstance() {
    static SystemManager instance;
    return instance;
}

/* =========================================================
   Main Menu
   ========================================================= */

void SystemManager::mainMenu() {
    int choice;

    do {
        cout << "\n===== SKILL-BASED JOB MATCHING SYSTEM =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            registerUser();
            break;

        case 2: {
            User* user = loginUser();
            if (user) {
                user->displayMenu();
            }
            break;
        }

        case 0:
            saveData();
            cout << "Exiting system...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

/* =========================================================
   Authentication
   ========================================================= */

void SystemManager::registerUser() {
    int roleChoice;
    string username, password, email;

    cout << "\nRegister as:\n";
    cout << "1. Candidate\n";
    cout << "2. Employer\n";
    cout << "3. Admin\n";
    cout << "Choice: ";
    cin >> roleChoice;

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    cout << "Email: ";
    cin >> email;

    int id = generateUserId();

    if (roleChoice == 1) {
        candidates[email] = new Candidate(id, username, password, email);
    }
    else if (roleChoice == 2) {
        employers[email] = new Employer(id, username, password, email);
    }
    else if (roleChoice == 3) {
        admins[email] = new Admin(id, username, password, email);
    }
    else {
        cout << "Invalid role selection.\n";
        return;
    }

    cout << "Registration successful.\n";
}

User* SystemManager::loginUser() {
    string email, password;

    cout << "\nEmail: ";
    cin >> email;
    cout << "Password: ";
    cin >> password;

    if (candidates.count(email) &&
        candidates[email]->authenticate(password))
        return candidates[email];

    if (employers.count(email) &&
        employers[email]->authenticate(password))
        return employers[email];

    if (admins.count(email) &&
        admins[email]->authenticate(password))
        return admins[email];

    cout << "Invalid credentials.\n";
    return nullptr;
}

/* =========================================================
   Job Management
   ========================================================= */

void SystemManager::addJob(Job* job) {
    jobs[job->getID()] = job;
}

void SystemManager::approveJob(int jobId) {
    if (jobs.count(jobId)) {
        cout << "Job approval can only be performed by Admin.\n";
    } else {
        cout << "Job not found.\n";
    }
}

const unordered_map<int, Job*>& SystemManager::getAllJobs() const {
    return jobs;
}

/* =========================================================
   Application Management
   ========================================================= */

void SystemManager::submitApplication(int candidateId,
                                      int jobId,
                                      double /* matchScore */) {
    Application* app = new Application(
        candidateId,
        UserRole::Candidate,
        jobId
    );

    applications.push_back(app);
    cout << "Application submitted successfully.\n";
}

vector<Application*> SystemManager::getApplicationsForJob(int jobId) const {
    vector<Application*> result;

    for (Application* app : applications) {
        if (app->jobId == jobId) {
            result.push_back(app);
        }
    }

    return result;
}

/* =========================================================
   Matching Coordination
   ========================================================= */

vector<int> SystemManager::getTopMatchingJobs(
    const Candidate& candidate,
    int topN
) {
    // Convert Job* map to Job map (MatchingEngine expects objects)
    unordered_map<int, Job> jobObjects;

    for (const auto& pair : jobs) {
        jobObjects[pair.first] = *(pair.second);
    }

    return MatchingEngine::getTopMatchingJobs(
        candidate,
        jobObjects,
        topN
    );
}

vector<string> SystemManager::getSkillGap(
    const Candidate& candidate,
    int jobId
) const {
    if (!jobs.count(jobId))
        return {};

    return MatchingEngine::getSkillGap(
        candidate,
        *(jobs.at(jobId))
    );
}

/* =========================================================
   File Handling (Skeleton)
   ========================================================= */

void SystemManager::loadData() {
    cout << "Loading system data...\n";
}

void SystemManager::saveData() {
    cout << "Saving system data...\n";
}

/* =========================================================
   ID Generation
   ========================================================= */

int SystemManager::generateUserId() {
    return nextUserId++;
}

int SystemManager::generateJobId() {
    return nextJobId++;
}

int SystemManager::generateApplicationId() {
    return nextApplicationId++;
}
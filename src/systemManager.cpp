#include "systemManager.h"
#include <conio.h>
#include "user.h"
#include "candidate.h"
#include "employer.h"
#include "admin.h"
#include "job.h"
#include "application.h"
#include "matchingengine.h"

#include <iostream>

using namespace std;

/*Masked Password Input*/

std::string getMaskedPassword() {
    std::string password;
    char ch;

    while ((ch = _getch()) != '\r') {  // Enter key
        if (ch == '\b') {              // Backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } else {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
}
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
        cout << "3. Exit\n";
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

        case 3:
            saveData();
            cout << "Exiting system...\n";
            return;
            //break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (true);
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

    // cin>> ws; // Clear input buffer before getline

    cout << "Username: ";
    getline(cin>>ws, username);
    cout << "Email: ";
    getline(cin>>ws, email);
   
    cout << "Password: ";
    password = getMaskedPassword();

    
    // ✅ EMAIL UNIQUENESS CHECK
    if (candidates.count(email) ||
        employers.count(email) ||
        admins.count(email)) {

        cout << "Error: This email is already registered.\n";
        return;
    }

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
    getline(cin>>ws, email);
    cout << "Password: ";
    password = getMaskedPassword();

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
   File Handling 
   ========================================================= */

void SystemManager::loadData() {
    std::ifstream fin("users.txt");

    if (!fin.is_open()) {
        std::cout << "No existing data found. Starting fresh.\n";
        return;
    }

    // Clear existing in-memory data
    candidates.clear();
    employers.clear();
    admins.clear();

    int id;
    std::string username, role, email;
    size_t passwordHash;

    while (fin >> id >> username >> passwordHash >> role >> email) {

        if (role == "candidate") {
            Candidate* c = new Candidate(id, username, "", email);
            c->loadFromFile(fin);
            candidates[email] = c;
        }
        else if (role == "employer") {
            Employer* e = new Employer(id, username, "", email);
            e->loadFromFile(fin);
            employers[email] = e;
        }
        else if (role == "admin") {
            Admin* a = new Admin(id, username, "", email);
            a->loadFromFile(fin);
            admins[email] = a;
        }

        // Make sure future IDs don't collide
        nextUserId = std::max(nextUserId, id + 1);
    }

    fin.close();
    std::cout << "System data loaded successfully.\n";
}

void SystemManager::saveData() {
    std::ofstream fout("users.txt");

    for (auto& [email, candidate] : candidates) {
        candidate->saveToFile(fout);
    }

    for (auto& [email, employer] : employers) {
        employer->saveToFile(fout);
    }

    for (auto& [email, admin] : admins) {
        admin->saveToFile(fout);
    }

    fout.close();
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
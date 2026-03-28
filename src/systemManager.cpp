#include "systemManager.h"
#include <conio.h>
#include "user.h"
#include "candidate.h"
#include "employer.h"
#include "admin.h"
#include "job.h"
#include "application.h"
#include "matchingengine.h"
#include <iomanip>

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
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');   // reset fill character 
        cout << setw(17) << "Welcome to Skill Based Job Recommendation System" << endl;
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');
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
    } while (choice != 3);
}

/* =========================================================
   Authentication
   ========================================================= */

void SystemManager::registerUser() {
    int roleChoice;
    string username, password, email;
    int id = generateUserId();

    cout << setfill('=') << setw(100) << "=" << endl;
    cout << setfill(' ');
    cout << setw(17) << "Register as: " << endl;
    cout << setfill('=') << setw(100) << "=" << endl;
    cout << setfill(' ');  
    cout << "1. Candidate\n";
    cout << "2. Employer\n";
    cout << "Choice: ";
    cin >> roleChoice;

    if (roleChoice == 1) {
        candidates[email] = new Candidate(id, username, password, email);
    }
    else if (roleChoice == 2) {
        employers[email] = new Employer(id, username, password, email);
    }
    else {
        cout << "Invalid role selection.\n";
        return;
    }

    // cin>> ws; // Clear input buffer before getline
    cout << setfill('=') << setw(100) << "=" << endl;
    cout << setfill(' ');

    cout << "Username: ";
    getline(cin>>ws, username);
    cout << "Email: ";
    getline(cin>>ws, email);
   
    cout << "Password: ";
    password = getMaskedPassword();

    cout << setfill('=') << setw(100) << "=" << endl;
    cout << setfill(' ');

    // ✅ EMAIL UNIQUENESS CHECK
    if (candidates.count(email) ||
        employers.count(email) ) {

        cout << "Error: This email is already registered.\n";
        return;
    }

    cout << "Registration successful.\n";

    cout << setfill('=') << setw(100) << "=" << endl;
    cout << setfill(' ');
}

User* SystemManager::loginUser() {
    string email, password;

    cout << setfill('=') << setw(100) << "=" << endl;
    cout << setfill(' ');

    cout << "\nEmail: ";
    getline(cin>>ws, email);
    cout << "Password: ";
    password = getMaskedPassword();

    // Check Candidate login
    if (candidates.count(email) &&
        candidates[email]->authenticate(password))
        return candidates[email];

    // Check Employer login
    if (employers.count(email) &&
        employers[email]->authenticate(password))
        return employers[email];

    // Check Admin login using hardcoded credentials
    if (Admin::validateAdminCredentials(email, password)) {
        // If admin doesn't exist in the map, create one
        if (!admins.count(email)) {
            int id = generateUserId();
            admins[email] = new Admin(id, Admin::ADMIN_USERNAME, Admin::ADMIN_PASSWORD, email);
        }
        cout << "Admin access granted.\n";
        return admins[email];
    }

    cout << "Invalid credentials.\n";
    cout << setfill('=') << setw(100) << "=" << endl;
    cout << setfill(' ');
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

// Used by edit jobs and clean null check
Job* SystemManager::getJobById(int jobId) {
    auto it = jobs.find(jobId);
    if (it != jobs.end())
        return it->second;
    return nullptr;
}

//Remove Jobs

bool SystemManager::removeJob(int jobId) {
    auto it = jobs.find(jobId);
    if (it == jobs.end())
        return false;

    delete it->second;
    jobs.erase(it);
    approvedJobs.erase(jobId);
    return true;
}



/* =========================================================
   Application Management
   ========================================================= */

void SystemManager::submitApplication(int candidateId, int jobId, double matchScore) {
    int appId = nextApplicationId++;
    Application* app = new Application(appId, candidateId, jobId);
    applications.push_back(app);
    cout << "Application submitted successfully.\n";
}


vector<Application*> SystemManager::getApplicationsForJob(int jobId) {
    std::vector<Application*> result;

    for (Application* app : applications) {
        if (app->getJobId() == jobId) {
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
    std::ifstream fin("usersdB.txt");

    if (!fin.is_open()) {
        std::cout << "No existing data found. Starting fresh.\n";
        return;
    }

    // Clear existing in-memory data
    candidates.clear();
    employers.clear();
    admins.clear();

    

    while (true) {
        int id;
        std::string username, role, email;
        size_t passwordHash;

        fin >> id >> username >> passwordHash >> role >> email;

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
    std::ofstream fout("usersdB.txt");

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

/* =========================================================
   Admin Accessors Implementation
   ========================================================= */

   std::unordered_map<std::string, Candidate*>&
SystemManager::getCandidates() {
    return candidates;
}

std::unordered_map<std::string, Employer*>&
SystemManager::getEmployers() {
    return employers;
}

std::unordered_map<int, Job*>&
SystemManager::getAllJobs() {
    return jobs;
}

std::unordered_set<int>&
SystemManager::getApprovedJobs() {
    return approvedJobs;
}
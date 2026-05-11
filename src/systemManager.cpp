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
#include <windows.h>
#include <iostream>
#include "utils.h"

using namespace std;

/*Masked Password Input*/

std::string getMaskedPassword() {
    std::string password;
    char ch;

    while (true) {

        ch = _getch();

        // Enter key
        if (ch == '\r') {

            if (password.length() < 8) {
                SetConsoleTextAttribute(h, 12);

                std::cout << "\nPassword must be at least 8 characters long.\n";
                std::cout << "Re-enter password: ";

                SetConsoleTextAttribute(h, 7);

                password.clear();
            }
            else {
                break;
            }
        }

        // Backspace
        else if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        }

        // Normal character
        else {
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
        printLine('=');
        SetConsoleTextAttribute(h, 2);
        centerText("Welcome to Skill Based Job Recommendation System");
        SetConsoleTextAttribute(h, 7);
        printLine('=');
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        printLine('=');
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
            SetConsoleTextAttribute(h, 14);
            cout << "Exiting system...\n";
            SetConsoleTextAttribute(h, 7);
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

    printLine('=');
    SetConsoleTextAttribute(h, 2);
    centerText("Register as: ");
    SetConsoleTextAttribute(h, 7);
    printLine('='); 

    SetConsoleTextAttribute(h, 13);
    cout << "1. Candidate\n";
    cout << "2. Employer\n";
    cout << "Choice: ";

    SetConsoleTextAttribute(h, 7);
    cin >> roleChoice;

    // cin>> ws; // Clear input buffer before getline

    cout << "Username: ";
    getline(cin>>ws, username);
    cout << "Email: ";
    getline(cin>>ws, email);
    if (!isValidEmail(email)) {
        SetConsoleTextAttribute(h, 12);

        cout << "Invalid email format.\n";

        SetConsoleTextAttribute(h, 7);
        return;
    }
    cout << "Password: ";
    password = getMaskedPassword();

    
    // EMAIL UNIQUENESS CHECK
    if (candidates.count(email) ||
        employers.count(email) ) {

        cout << "Error: This email is already registered.\n";
        return;
    }

    int id = generateUserId();

    if (roleChoice == 1) {
        candidates[email] = new Candidate(id, username, password, email);
        cout << "Candidate " << username<<"(" << email << ")" << " registered successfully!\n";
        saveData();  // Auto-save after registration
    }
    else if (roleChoice == 2) {
        employers[email] = new Employer(id, username, password, email);
        cout << "Employer " << username << "(" << email << ")" << " registered successfully!\n";
        saveData();  // Auto-save after registration
    }
    else {
        cout << "Invalid role selection.\n";
        return;
    }

    cout << "Registration successful.\n";

    printLine('=');
}

User* SystemManager::loginUser() {
    string email, password;

    printLine('=');
    SetConsoleTextAttribute(h, 2);
    centerText("Login Dashboard");
    SetConsoleTextAttribute(h, 7);
    printLine('=');

    cout << "\nEmail: ";
    getline(cin>>ws, email);
    cout << "Password: ";
    password = getMaskedPassword();

    // Check Candidate login
    if (candidates.count(email) &&
        candidates[email]->authenticate(password)){
        return candidates[email];
        printLine(':');
        cout << candidates[email]->getUsername() << " logged in successfully!\n";
        printLine(':');
        }

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
    printLine(':');
    SetConsoleTextAttribute(h, 12);
    centerText("Invalid credentials.");
    SetConsoleTextAttribute(h, 7);
    printLine(':');
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

void SystemManager::submitApplication(int candidateId, int jobId, double matchScore, const std::vector<std::string>& screeningAnswers) {
    int appId = nextApplicationId++;
    Application* app = new Application(appId, candidateId, jobId, screeningAnswers);
    applications.push_back(app);
    cout << "Application submitted successfully.\n";
    saveData();  // Auto-save after application submission
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

// Helper to find candidate by ID (used in employer view applicants)

Candidate* SystemManager::findCandidateById(int id) {

    for (auto& pair : candidates) {

        Candidate* c = pair.second;

        if (c->getID() == id) {
            return c;
        }
    }

    return nullptr;
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

vector<pair<int, double>> SystemManager::getTopMatchingJobsWithScores(
    const Candidate& candidate,
    int topN
) {
    // Convert Job* map to Job map (MatchingEngine expects objects)
    unordered_map<int, Job> jobObjects;

    for (const auto& pair : jobs) {
        jobObjects[pair.first] = *(pair.second);
    }

    return MatchingEngine::getTopMatchingJobsWithScores(
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
    candidates.clear();
    employers.clear();
    admins.clear();
    jobs.clear();
    applications.clear();
    approvedJobs.clear();

    // Load candidate data
    {
        std::ifstream fin("candidateDB.csv");
        if (fin.is_open()) {
            std::string line;
            while (std::getline(fin, line)) {
                if (line.empty()) continue;
                Candidate* c = new Candidate();
                c->fromCSV(line);
                candidates[c->getEmail()] = c;
                nextUserId = std::max(nextUserId, c->getID() + 1);
            }
            fin.close();
        }
    }

    // Load employer data
    {
        std::ifstream fin("employerDB.csv");
        if (fin.is_open()) {
            std::string line;
            while (std::getline(fin, line)) {
                if (line.empty()) continue;
                Employer* e = new Employer();
                e->fromCSV(line);
                std::string email = e->getEmail();
                if (!email.empty() && !employers.count(email)) {
                    employers[email] = e;
                    nextUserId = std::max(nextUserId, e->getID() + 1);
                } else {
                    delete e;
                }
            }
            fin.close();
        }
    }

    // Load job data
    {
        std::ifstream fin("jobDB.csv");
        if (fin.is_open()) {
            std::string line;
            while (std::getline(fin, line)) {
                if (line.empty()) continue;
                Job* j = new Job();
                j->fromCSV(line);
                jobs[j->getID()] = j;
                approvedJobs.insert(j->getID());
                nextJobId = std::max(nextJobId, j->getID() + 1);
            }
            fin.close();
        }
    }

    // Load application data
    {
        std::ifstream fin("applicationDB.csv");
        if (fin.is_open()) {
            std::string line;
            while (std::getline(fin, line)) {
                if (line.empty()) continue;
                Application* a = new Application();
                a->fromCSV(line);
                applications.push_back(a);
                nextApplicationId = std::max(nextApplicationId, a->getApplicationId() + 1);
            }
            fin.close();
        }
    }

    std::cout << "System data loaded successfully.\n";
}

void SystemManager::saveData() {
    {
        std::ofstream fout("candidateDB.csv");
        for (auto& [email, candidate] : candidates) {
            fout << candidate->toCSV() << "\n";
        }
        fout.close();
    }

    {
        std::ofstream fout("employerDB.csv");
        for (auto& [email, employer] : employers) {
            fout << employer->toCSV() << "\n";
        }
        fout.close();
    }

    {
        std::ofstream fout("jobDB.csv");
        for (auto& [id, job] : jobs) {
            fout << job->toCSV() << "\n";
        }
        fout.close();
    }

    {
        std::ofstream fout("applicationDB.csv");
        for (Application* app : applications) {
            fout << app->toCSV() << "\n";
        }
        fout.close();
    }

    // Admin is static user; no CSV persistence needed for hardcoded admin user.
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
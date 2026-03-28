#include "employer.h"
#include "systemManager.h"
#include "job.h"
#include "application.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

static int parseIntInput(const std::string& text, int fallback = 0);
static int readIntWithPrompt(const std::string& prompt, int fallback = 0);

/* =========================
   Constructor
   ========================= */

Employer::Employer(
    int id,
    string username,
    string password,
    string email,
    string companyName
)
    : User(id, username, password, "employer", email),
      companyName(companyName) {}


/* =========================
   Post Job
   ========================= */

void Employer::postJob() {
    SystemManager& system = SystemManager::getInstance();

    int jobId = system.generateJobId();
    Job* job = new Job(jobId, "", getID());  // Empty title initially, will be set below

    cout << "\n======== FORMAL JOB POSTING ========\n";

    // Basic Information
    string title, description;
    cout << "Job Title: ";
    getline(cin >> ws, title);
    job->setTitle(title);

    cout << "Job Description: ";
    getline(cin >> ws, description);
    job->setDescription(description);

    job->setCompanyName(companyName);

    string location;
    cout << "Location: ";
    getline(cin >> ws, location);
    job->setLocation(location);

    // Compensation & Type
    string salaryRange;
    cout << "Salary Range (e.g., '$50,000 - $70,000 per year'): ";
    getline(cin >> ws, salaryRange);
    job->setSalaryRange(salaryRange);

    int jobTypeChoice;
    cout << "Job Type:\n";
    cout << "1. Full-time\n";
    cout << "2. Part-time\n";
    cout << "3. Contract\n";
    cout << "4. Internship\n";
    cout << "Choice: ";
    cin >> jobTypeChoice;

    JobType jobType = JobType::FullTime;
    if (jobTypeChoice == 2) jobType = JobType::PartTime;
    else if (jobTypeChoice == 3) jobType = JobType::Contract;
    else if (jobTypeChoice == 4) jobType = JobType::Internship;
    job->setJobType(jobType);

    if (jobType == JobType::Contract) {
        string duration;
        cout << "Contract Duration: ";
        getline(cin >> ws, duration);
        job->setContractDuration(duration);
    }

    // Skills Specification
    cout << "\n=== REQUIRED SKILLS ===\n";
    int reqSkillCount = readIntWithPrompt("How many required skills? (0/none allowed) ", 0);

    for (int i = 0; i < reqSkillCount; i++) {
        string skill;
        cout << "Required skill " << (i + 1) << ": ";
        getline(cin >> ws, skill);
        if (!skill.empty()) {
            job->addRequiredSkill(skill);
        }
    }

    cout << "\n=== OPTIONAL SKILLS ===\n";
    int optSkillCount = readIntWithPrompt("How many optional skills? (0/none allowed) ", 0);

    for (int i = 0; i < optSkillCount; i++) {
        string skill;
        cout << "Optional skill " << (i + 1) << ": ";
        getline(cin >> ws, skill);
        if (!skill.empty()) {
            job->addOptionalSkill(skill);
        }
    }

    // Additional Information
    string contactInfo;
    cout << "Contact Information (email/phone): ";
    getline(cin >> ws, contactInfo);
    job->setContactInformation(contactInfo);

    cout << "\n=== SCREENING QUESTIONS ===\n";
    int questionCount = readIntWithPrompt("How many screening questions? (0/none allowed) ", 0);

    for (int i = 0; i < questionCount; i++) {
        string question;
        cout << "Question " << (i + 1) << ": ";
        getline(cin >> ws, question);
        if (!question.empty() && question != "none" && question != "None") {
            job->addScreeningQuestion(question);
        }
    }

    system.addJob(job);
    postedJobIDs.push_back(jobId);

    cout << "\n✅ Job posted successfully with ID " << jobId << "!\n";
    system.saveData();  // Auto-save after job posting
}


/* =========================
   Edit Job
   ========================= */

void Employer::editJob(int jobId) {
    SystemManager& system = SystemManager::getInstance();
    Job* job = system.getJobById(jobId);

    if (!job) {
        cout << "Job not found.\n";
        return;
    }

    string newTitle;
    cout << "Enter new job title: ";
    getline(cin >> ws, newTitle);
    job->setTitle(newTitle);

    cout << "Clear and re‑enter skills? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        job->clearSkills();

        int skillCount;
        cout << "How many required skills? ";
        cin >> skillCount;

        for (int i = 0; i < skillCount; i++) {
            string skill;
            cout << "Enter skill " << i + 1 << ": ";
            getline(cin >> ws, skill);
            job->addRequiredSkill(skill);
        }
    }

    cout << "Job updated successfully.\n";
}


/* =========================
   Delete Job
   ========================= */

void Employer::deleteJob(int jobId) {
    SystemManager& system = SystemManager::getInstance();

    if (!system.removeJob(jobId)) {
        cout << "Job not found.\n";
        return;
    }

    postedJobIDs.erase(
        remove(postedJobIDs.begin(), postedJobIDs.end(), jobId),
        postedJobIDs.end()
    );

    cout << "Job deleted successfully.\n";
}


/* =========================
   View Applicants
   ========================= */

void Employer::viewApplicants(int jobId) const {
    SystemManager& system = SystemManager::getInstance();
    auto apps = system.getApplicationsForJob(jobId);

    if (apps.empty()) {
        cout << "No applications for this job yet.\n";
        return;
    }

    cout << "\nApplicants for Job ID " << jobId << ":\n";

    for (const Application* app : apps) {
        cout << "Candidate ID: " << app->getCandidateId()
             << " | Status: "
             << Application::statusToString(app->getStatus())
             << "\n";
    }
}


/* =========================
   Getters
   ========================= */

string Employer::getCompanyName() const {
    return companyName;
}

const vector<int>& Employer::getPostedJobIDs() const {
    return postedJobIDs;
}


/* =========================
   Menu (SESSION LOOP ✅)
   ========================= */

void Employer::displayMenu() {
    int choice;

    do {
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');   
        cout << setw(38) <<" " << "Employer Dashboard" << endl;
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');
        cout << "1. Post Job\n";
        cout << "2. Edit Job\n";
        cout << "3. Delete Job\n";
        cout << "4. View Applicants\n";
        cout << "5. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            postJob();
            break;

        case 2: {
            int jobId;
            cout << "Enter Job ID to edit: ";
            cin >> jobId;
            editJob(jobId);
            break;
        }

        case 3: {
            int jobId;
            cout << "Enter Job ID to delete: ";
            cin >> jobId;
            deleteJob(jobId);
            break;
        }

        case 4: {
            int jobId;
            cout << "Enter Job ID to view applicants: ";
            cin >> jobId;
            viewApplicants(jobId);
            break;
        }

        case 5:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);
}


/* =========================
   File Handling
   ========================= */

/* =========================================================
   Helper functions
   ========================================================= */

static std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

static std::string joinString(const std::vector<std::string>& items, char delimiter) {
    std::string result;
    for (size_t i = 0; i < items.size(); ++i) {
        result += items[i];
        if (i + 1 < items.size())
            result.push_back(delimiter);
    }
    return result;
}

static int parseIntInput(const std::string& text, int fallback) {
    if (text.empty()) return fallback;
    if (text == "none" || text == "None" || text == "NONE") return 0;
    try {
        size_t idx;
        int value = std::stoi(text, &idx);
        return (idx == text.size()) ? value : fallback;
    } catch (...) {
        return fallback;
    }
}

static int readIntWithPrompt(const std::string& prompt, int fallback) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, input);
    int value = parseIntInput(input, fallback);
    return value < 0 ? fallback : value;
}

/* =========================================================
   CSV Persistence
   ========================================================= */

std::string Employer::toCSV() const {
    std::vector<std::string> posted;
    for (int id : postedJobIDs) {
        posted.push_back(std::to_string(id));
    }
    return std::to_string(id) + "," + username + "," + std::to_string(passwordHash) + "," + role + "," + email + "," + companyName + "," + joinString(posted, '|');
}

void Employer::fromCSV(const std::string& csvLine) {
    auto fields = splitString(csvLine, ',');
    if (fields.size() < 7) {
        return;
    }

    id = std::stoi(fields[0]);
    username = fields[1];
    passwordHash = static_cast<size_t>(std::stoull(fields[2]));
    role = fields[3];
    email = fields[4];
    companyName = fields[5];

    postedJobIDs.clear();
    if (!fields[6].empty()) {
        auto tokens = splitString(fields[6], '|');
        for (const auto& token : tokens) {
            if (!token.empty()) {
                postedJobIDs.push_back(std::stoi(token));
            }
        }
    }
}

/* =========================================================
   File Handling
   ========================================================= */

void Employer::saveToFile(ofstream& out) const {
    out << toCSV() << "\n";
}

void Employer::loadFromFile(ifstream& in) {
    std::string line;
    std::getline(in, line);
    if (line.empty())
        return;
    fromCSV(line);
}

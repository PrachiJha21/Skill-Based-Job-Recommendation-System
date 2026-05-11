#include "employer.h"
#include "systemManager.h"
#include "job.h"
#include "application.h"
#include "candidate.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "utils.h"

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

    printLine('=');
    centerText("FORMAL JOB POSTING");
    printLine('=');

    // Basic Information
    string title, description, companyName;
    cout << "Job Title: ";
    getline(cin >> ws, title);
    job->setTitle(title);

    cout << "Job Description: ";
    getline(cin >> ws, description);
    job->setDescription(description);

    cout << "Company Name: ";
    getline(cin >> ws, companyName);
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
    printLine('-');
    centerText("Job Type");
    printLine('-');
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
    printLine('.');
    centerText("REQUIRED SKILLS");
    printLine('.');
    int reqSkillCount = readIntWithPrompt("How many required skills? (0/none allowed) ", 0);

    for (int i = 0; i < reqSkillCount; i++) {
        string skill;
        cout << "Required skill " << (i + 1) << ": ";
        getline(cin >> ws, skill);
        if (!skill.empty()) {
            job->addRequiredSkill(skill);
        }
    }

    printLine('.');
    centerText("OPTIONAL SKILLS");
    printLine('.');
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

    printLine('.');
    centerText("SCREENING QUESTIONS");
    printLine('.');
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

    SetConsoleTextAttribute(h, 10);
    centerText("Job posted successfully with JobID =" + to_string(jobId));
    SetConsoleTextAttribute(h, 7);
    system.saveData();  // Auto-save after job posting
}


/* =========================
   Edit Job
   ========================= */

void Employer::editJob(int jobId) {
    SystemManager& system = SystemManager::getInstance();
    Job* job = system.getJobById(jobId);

    if (!job) {
        SetConsoleTextAttribute(h, 12);
        centerText("Job not found!");
        SetConsoleTextAttribute(h, 7);
        return;
    }

    string newTitle;
    cout << "Enter new job title: ";
    getline(cin >> ws, newTitle);
    job->setTitle(newTitle);
    printLine('-');
    cout << "Clear and enter skills once again? (y/n): ";
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

    SetConsoleTextAttribute(h, 10);
    centerText("Job updated successfully.");
    SetConsoleTextAttribute(h, 7);
}


/* =========================
   Delete Job
   ========================= */

void Employer::deleteJob(int jobId) {
    SystemManager& system = SystemManager::getInstance();

    if (!system.removeJob(jobId)) {
        SetConsoleTextAttribute(h, 12);
        centerText("Job not found or could not be deleted.");
        SetConsoleTextAttribute(h, 7);
        return;
    }

    postedJobIDs.erase(
        remove(postedJobIDs.begin(), postedJobIDs.end(), jobId),
        postedJobIDs.end()
    );

    SetConsoleTextAttribute(h, 10);
    centerText("Job deleted successfully.");
    SetConsoleTextAttribute(h, 7);
}


/* =========================
   View Applicants
   ========================= */

void Employer::viewApplicants(int jobId) const {
    SystemManager& system = SystemManager::getInstance();
    auto apps = system.getApplicationsForJob(jobId);

    if (apps.empty()) {
        centerText("No applications for this job yet.");
        return;
    }

    printLine('-');
    centerText("Applicants for Job ID " + to_string(jobId));
    printLine('-');

    for (const Application* app : apps) {

        // Get candidate using candidate ID
        Candidate* candidate =
            system.findCandidateById(app->getCandidateId());

        if (candidate == nullptr) {
            continue;
        }

        printLine('-');

        // Display full profile
        cout << "Candidate ID: " << candidate->getID() << "\n";
        cout << "Name: " << candidate->getUsername() << "\n";
        cout << "Email: " << candidate->getEmail() << "\n";
        
        //Skill Printing
        cout << "Skills:\n";

        for (const auto& skillPair : candidate->getSkills()) {
            cout << "- " << skillPair.first << " : ";
            switch (skillPair.second) {
                case SkillLevel::Beginner:
                    cout << "Beginner";
                    break;

                case SkillLevel::Intermediate:
                    cout << "Intermediate";
                    break;

                case SkillLevel::Expert:
                    cout << "Expert";
                    break;
            }
            cout << "\n";
        }

        //Education Printing

        cout << "\nEducation:\n";

        for (const string& edu : candidate->getEducation()) {
            cout << "- " << edu << "\n";
        }
        
        //Experience Printing

        cout << "\nExperience:\n";

        for (const string& exp : candidate->getExperience()) {
            cout << "- " << exp << "\n";
        }


        // Application info
        cout << "Application Status: "
             << Application::statusToString(app->getStatus())
             << "\n";

        // Screening answers
        vector<string> answers = app->getScreeningAnswers();

        if (!answers.empty()) {
            cout << "\nScreening Answers:\n";

            for (size_t i = 0; i < answers.size(); ++i) {
                cout << "Q" << (i + 1)
                     << ": " << answers[i] << "\n";
            }
        }

        printLine('-');
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
   Menu (SESSION LOOP )
   ========================= */

void Employer::displayMenu() {
    int choice;

    do {
        printLine('=');
        SetConsoleTextAttribute(h, 3);
        centerText("Employer Dashboard");
        SetConsoleTextAttribute(h, 7);
        printLine('=');
        SetConsoleTextAttribute(h, 5);
        cout << "1. Post Job\n";
        cout << "2. Edit Job\n";
        cout << "3. Delete Job\n";
        cout << "4. View Applicants\n";
        cout << "5. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        SetConsoleTextAttribute(h, 7);
        
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

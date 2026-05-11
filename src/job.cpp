#include "job.h"
#include "utils.h"
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

/* =========================
   Constructor
   ========================= */

Job::Job(
    int id,
    std::string title,
    int employerId
)
    : id(id),
      employerId(employerId),
      title(title),
      jobType(JobType::FullTime) {}

/* =========================
   Getters
   ========================= */

int Job::getID() const {
    return id;
}

int Job::getEmployerId() const {
    return employerId;
}

std::string Job::getTitle() const {
    return title;
}

std::string Job::getDescription() const {
    return description;
}

std::string Job::getCompanyName() const {
    return companyName;
}

std::string Job::getLocation() const {
    return location;
}

std::string Job::getSalaryRange() const {
    return salaryRange;
}

JobType Job::getJobType() const {
    return jobType;
}

std::string Job::getContractDuration() const {
    return contractDuration;
}

std::string Job::getContactInformation() const {
    return contactInformation;
}

const std::vector<std::string>& Job::getScreeningQuestions() const {
    return screeningQuestions;
}

/* =========================
   Setters
   ========================= */

void Job::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Job::setDescription(const std::string& desc) {
    description = desc;
}

void Job::setCompanyName(const std::string& company) {
    companyName = company;
}

void Job::setLocation(const std::string& loc) {
    location = loc;
}

void Job::setSalaryRange(const std::string& salary) {
    salaryRange = salary;
}

void Job::setJobType(JobType type) {
    jobType = type;
}

void Job::setContractDuration(const std::string& duration) {
    contractDuration = duration;
}

void Job::setContactInformation(const std::string& contact) {
    contactInformation = contact;
}

void Job::addScreeningQuestion(const std::string& question) {
    screeningQuestions.push_back(question);
}

/* =========================
   Skill management
   ========================= */

void Job::clearSkills() {
    requiredSkills.clear();
    optionalSkills.clear();
}

void Job::addRequiredSkill(const std::string& skill) {
    if (!skill.empty()) {
        requiredSkills.insert(skill);
    }
}

void Job::addOptionalSkill(const std::string& skill) {
    if (!skill.empty()) {
        optionalSkills.insert(skill);
    }
}

const std::unordered_set<std::string>& Job::getRequiredSkills() const {
    return requiredSkills;
}

const std::unordered_set<std::string>& Job::getOptionalSkills() const {
    return optionalSkills;
}

/* =========================
   Utility Functions
   ========================= */

std::string Job::jobTypeToString(JobType type) {
    switch (type) {
        case JobType::FullTime: return "Full-time";
        case JobType::PartTime: return "Part-time";
        case JobType::Contract: return "Contract";
        case JobType::Internship: return "Internship";
        default: return "Full-time";
    }
}

JobType Job::stringToJobType(const std::string& typeStr) {
    if (typeStr == "Part-time") return JobType::PartTime;
    if (typeStr == "Contract") return JobType::Contract;
    if (typeStr == "Internship") return JobType::Internship;
    return JobType::FullTime;
}

void Job::displayJobDetails() const {
    printLine('-');
    centerText("Job Details");
    printLine('-');
    cout << "Job ID: " << id << "\n";
    cout << "Title: " << title << "\n";
    cout << "Company: " << companyName << "\n";
    cout << "Location: " << location << "\n";
    cout << "Job Type: " << jobTypeToString(jobType) << "\n";
    cout << "Salary Range: " << salaryRange << "\n";
    if (!contractDuration.empty()) {
        cout << "Contract Duration: " << contractDuration << "\n";
    }
    cout << "\nDescription:\n" << description << "\n";

    cout << "\nRequired Skills:\n";
    for (const auto& skill : requiredSkills) {
        cout << "- " << skill << "\n";
    }

    if (!optionalSkills.empty()) {
        cout << "\nOptional Skills:\n";
        for (const auto& skill : optionalSkills) {
            cout << "- " << skill << "\n";
        }
    }

    cout << "\nContact Information: " << contactInformation << "\n";

    if (!screeningQuestions.empty()) {
        cout << "\nScreening Questions:\n";
        for (size_t i = 0; i < screeningQuestions.size(); ++i) {
            cout << (i + 1) << ". " << screeningQuestions[i] << "\n";
        }
    }
}

/* =========================
   File Handling
   ========================= */

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

std::string Job::toCSV() const {
    std::vector<std::string> req(requiredSkills.begin(), requiredSkills.end());
    std::vector<std::string> opt(optionalSkills.begin(), optionalSkills.end());

    std::string reqCsv = joinString(req, '|');
    std::string optCsv = joinString(opt, '|');
    std::string questionsCsv = joinString(screeningQuestions, '|');

    return std::to_string(id) + "," + std::to_string(employerId) + "," + title + "," + description + "," +
           companyName + "," + location + "," + salaryRange + "," + jobTypeToString(jobType) + "," +
           contractDuration + "," + reqCsv + "," + optCsv + "," + contactInformation + "," + questionsCsv;
}

void Job::fromCSV(const std::string& csvLine) {
    auto fields = splitString(csvLine, ',');
    if (fields.size() < 14) {
        return;
    }

    id = std::stoi(fields[0]);
    employerId = std::stoi(fields[1]);
    title = fields[2];
    description = fields[3];
    companyName = fields[4];
    location = fields[5];
    salaryRange = fields[6];
    jobType = stringToJobType(fields[7]);
    contractDuration = fields[8];

    // Required skills
    requiredSkills.clear();
    if (!fields[9].empty()) {
        auto reqTokens = splitString(fields[9], '|');
        for (const auto& token : reqTokens) {
            if (!token.empty())
                requiredSkills.insert(token);
        }
    }

    // Optional skills
    optionalSkills.clear();
    if (!fields[10].empty()) {
        auto optTokens = splitString(fields[10], '|');
        for (const auto& token : optTokens) {
            if (!token.empty())
                optionalSkills.insert(token);
        }
    }

    contactInformation = fields[11];

    // Screening questions
    screeningQuestions.clear();
    if (!fields[12].empty()) {
        screeningQuestions = splitString(fields[12], '|');
    }
}

void Job::saveToFile(std::ofstream& out) const {
    out << toCSV() << "\n";
}

void Job::loadFromFile(std::ifstream& in) {
    std::string line;
    std::getline(in, line);
    if (line.empty())
        return;
    fromCSV(line);
}

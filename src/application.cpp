#include "application.h"
#include <iostream>
#include <sstream>
#include <vector>

/* =========================
   Constructors
   ========================= */

// Default constructor (required for file loading)
Application::Application()
    : applicationId(0),
      candidateId(0),
      jobId(0),
      status(ApplicationStatus::Pending),
      appliedDate(0),
      updatedDate(0) {}

// Constructor for creating a new application
Application::Application(int applicationId, int candidateId, int jobId, const std::vector<std::string>& screeningAnswers)
    : applicationId(applicationId),
      candidateId(candidateId),
      jobId(jobId),
      status(ApplicationStatus::Pending),
      screeningAnswers(screeningAnswers) {

    appliedDate = std::time(nullptr);
    updatedDate = appliedDate;
}

/* =========================
   Getters
   ========================= */

int Application::getApplicationId() const {
    return applicationId;
}

int Application::getCandidateId() const {
    return candidateId;
}

int Application::getJobId() const {
    return jobId;
}

ApplicationStatus Application::getStatus() const {
    return status;
}

std::time_t Application::getAppliedDate() const {
    return appliedDate;
}

std::time_t Application::getUpdatedDate() const {
    return updatedDate;
}

const std::vector<std::string>& Application::getScreeningAnswers() const {
    return screeningAnswers;
}

/* =========================
   Status Management
   ========================= */

void Application::updateStatus(ApplicationStatus newStatus) {
    status = newStatus;
    updatedDate = std::time(nullptr);
}

/* =========================
   Persistence (applicationDB.txt)
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

std::string Application::toCSV() const {
    // Screening answers: answer1|answer2|answer3
    std::string answersCsv;
    for (size_t i = 0; i < screeningAnswers.size(); ++i) {
        answersCsv += screeningAnswers[i];
        if (i + 1 < screeningAnswers.size())
            answersCsv += "|";
    }

    return std::to_string(applicationId) + "," +
           std::to_string(candidateId) + "," +
           std::to_string(jobId) + "," +
           statusToString(status) + "," +
           std::to_string(appliedDate) + "," +
           std::to_string(updatedDate) + "," +
           answersCsv;
}

void Application::fromCSV(const std::string& csvLine) {
    auto fields = splitString(csvLine, ',');
    if (fields.size() < 7) {
        return;
    }

    applicationId = std::stoi(fields[0]);
    candidateId = std::stoi(fields[1]);
    jobId = std::stoi(fields[2]);
    status = stringToStatus(fields[3]);
    appliedDate = static_cast<std::time_t>(std::stoll(fields[4]));
    updatedDate = static_cast<std::time_t>(std::stoll(fields[5]));

    // Parse screening answers
    screeningAnswers.clear();
    if (!fields[6].empty()) {
        screeningAnswers = splitString(fields[6], '|');
    }
}

void Application::saveToFile(std::ofstream& out) const {
    out << toCSV() << "\n";
}

void Application::loadFromFile(std::ifstream& in) {
    std::string line;
    std::getline(in, line);
    if (line.empty())
        return;
    fromCSV(line);
}

/* =========================
   Utility Functions
   ========================= */

std::string Application::statusToString(ApplicationStatus status) {
    switch (status) {
        case ApplicationStatus::Pending:  return "Pending";
        case ApplicationStatus::Accepted: return "Accepted";
        case ApplicationStatus::Rejected: return "Rejected";
        default:                          return "Pending";
    }
}

ApplicationStatus Application::stringToStatus(const std::string& statusStr) {
    if (statusStr == "Accepted")
        return ApplicationStatus::Accepted;
    if (statusStr == "Rejected")
        return ApplicationStatus::Rejected;
    return ApplicationStatus::Pending;
}
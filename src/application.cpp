#include "application.h"
#include <iostream>

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
Application::Application(int applicationId, int candidateId, int jobId)
    : applicationId(applicationId),
      candidateId(candidateId),
      jobId(jobId),
      status(ApplicationStatus::Pending) {

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

void Application::saveToFile(std::ofstream& out) const {
    out << applicationId << " "
        << candidateId << " "
        << jobId << " "
        << statusToString(status) << " "
        << appliedDate << " "
        << updatedDate << "\n";
}

void Application::loadFromFile(std::ifstream& in) {
    std::string statusStr;

    in >> applicationId
       >> candidateId
       >> jobId
       >> statusStr
       >> appliedDate
       >> updatedDate;

    status = stringToStatus(statusStr);
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
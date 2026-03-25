#include "application.h"
#include <iostream>
#include <stdexcept>

// Constructor: sets status to Pending, records appliedDate, blocks non-candidates
Application::Application(int userId, UserRole userRole, int jobId) {
    if (userRole != UserRole::Candidate) {
        throw std::invalid_argument(
            "Only users with role Candidate can submit applications.");
    }
    this->userId      = userId;
    this->userRole    = userRole;
    this->jobId       = jobId;
    this->status      = ApplicationStatus::Pending;
    this->appliedDate = std::time(nullptr);   // current date/time
    this->updatedDate = this->appliedDate;
}

// Update status and record the updated date
void Application::updateStatus(ApplicationStatus newStatus) {
    status      = newStatus;
    updatedDate = std::time(nullptr);   // refresh updated date
    std::cout << "  [Status Updated] User " << userId
              << "'s application for Job " << jobId
              << " is now: " << statusToString(status) << "\n";
}

// Display all application details including both dates
void Application::display() const {
    char appliedBuf[26], updatedBuf[26];

    // ctime_s is safer but not always available; use ctime and trim newline
    std::string aDate = std::ctime(&appliedDate);
    std::string uDate = std::ctime(&updatedDate);
    if (!aDate.empty()) aDate.pop_back();   // remove trailing \n
    if (!uDate.empty()) uDate.pop_back();

    std::cout << "  User ID: "      << userId
              << " | Job ID: "      << jobId
              << " | Status: "      << statusToString(status)
              << " | Applied: "     << aDate
              << " | Updated: "     << uDate << "\n";
}

// Convert enum to string for printing
std::string Application::statusToString(ApplicationStatus s) {
    switch (s) {
        case ApplicationStatus::Pending:  return "Pending";
        case ApplicationStatus::Accepted: return "Accepted";
        case ApplicationStatus::Rejected: return "Rejected";
        default:                          return "Unknown";
    }
}
#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <ctime>

// Enum for user role -- only Candidate can apply
enum class UserRole {
    Candidate,
    Employer,
    Admin
};

// Enum for application status -- replaces plain string
enum class ApplicationStatus {
    Pending,
    Accepted,
    Rejected
};

class Application {
public:
    int userId;
    UserRole userRole;       // Must be Candidate to apply
    int jobId;
    ApplicationStatus status;
    std::time_t appliedDate;  // Date application was submitted
    std::time_t updatedDate;  // Date status was last changed

    // Constructor: requires userId, userRole, and jobId
    Application(int userId, UserRole userRole, int jobId);

    // Update the application status (only valid enum values accepted)
    void updateStatus(ApplicationStatus newStatus);

    // Display application info
    void display() const;

    // Helper: convert status enum to readable string
    static std::string statusToString(ApplicationStatus s);
};

#endif
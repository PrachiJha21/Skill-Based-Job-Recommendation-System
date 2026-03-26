#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <ctime>
#include <fstream>

/*
 Application
 - Represents a candidate applying to a job
 - Persisted permanently in applicationDB.txt
 - Independent from user storage (users.txt)
*/

// Application status lifecycle
enum class ApplicationStatus {
    Pending,
    Accepted,
    Rejected
};

class Application {
private:
    int applicationId;          // Unique application ID
    int candidateId;            // ID of the candidate who applied
    int jobId;                  // ID of the job applied to
    ApplicationStatus status;   // Current status of application
    std::time_t appliedDate;    // When application was created
    std::time_t updatedDate;    // Last status update time

public:
    /* =========================
       Constructors
       ========================= */

    Application();  // Required for loading from file

    Application(
        int applicationId,
        int candidateId,
        int jobId
    );

    /* =========================
       Getters
       ========================= */

    int getApplicationId() const;
    int getCandidateId() const;
    int getJobId() const;
    ApplicationStatus getStatus() const;
    std::time_t getAppliedDate() const;
    std::time_t getUpdatedDate() const;

    /* =========================
       Status Management
       ========================= */

    void updateStatus(ApplicationStatus newStatus);

    /* =========================
       Persistence (applicationDB.txt)
       ========================= */

    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);

    /* =========================
       Utility helpers
       ========================= */

    static std::string statusToString(ApplicationStatus status);
    static ApplicationStatus stringToStatus(const std::string& statusStr);
};

#endif // APPLICATION_H
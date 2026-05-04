#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <ctime>
#include <fstream>
#include <vector>

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
    std::vector<std::string> screeningAnswers;  // Answers to screening questions

public:
    /* =========================
       Constructors
       ========================= */

    Application();  // Required for loading from file

    Application(
        int applicationId,
        int candidateId,
        int jobId,
        const std::vector<std::string>& screeningAnswers = std::vector<std::string>()
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
    const std::vector<std::string>& getScreeningAnswers() const;

    /* =========================
       Status Management
       ========================= */

    void updateStatus(ApplicationStatus newStatus);

    /* =========================
       Persistence (applicationDB.csv)
       ========================= */

    std::string toCSV() const;
    void fromCSV(const std::string& csvLine);

    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);

    /* =========================
       Utility helpers
       ========================= */

    static std::string statusToString(ApplicationStatus status);
    static ApplicationStatus stringToStatus(const std::string& statusStr);
};

#endif // APPLICATION_H
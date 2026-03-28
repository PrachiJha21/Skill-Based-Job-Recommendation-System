#ifndef JOB_H
#define JOB_H

#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>

enum class JobType {
    FullTime,
    PartTime,
    Contract,
    Internship
};

class Job {
private:
    int id;
    int employerId;

    // Basic Information
    std::string title;
    std::string description;
    std::string companyName;
    std::string location;

    // Compensation & Type
    std::string salaryRange;
    JobType jobType;
    std::string contractDuration;  // if applicable

    // Skills Specification
    std::unordered_set<std::string> requiredSkills;
    std::unordered_set<std::string> optionalSkills;

    // Additional Information
    std::string contactInformation;
    std::vector<std::string> screeningQuestions;

public:
    Job(
        int id = 0,
        std::string title = "",
        int employerId = 0
    );

    // Getters
    int getID() const;
    int getEmployerId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getCompanyName() const;
    std::string getLocation() const;
    std::string getSalaryRange() const;
    JobType getJobType() const;
    std::string getContractDuration() const;
    std::string getContactInformation() const;
    const std::vector<std::string>& getScreeningQuestions() const;

    // Setters
    void setTitle(const std::string& newTitle);
    void setDescription(const std::string& desc);
    void setCompanyName(const std::string& company);
    void setLocation(const std::string& loc);
    void setSalaryRange(const std::string& salary);
    void setJobType(JobType type);
    void setContractDuration(const std::string& duration);
    void setContactInformation(const std::string& contact);
    void addScreeningQuestion(const std::string& question);

    // Skill management
    void clearSkills();
    void addRequiredSkill(const std::string& skill);
    void addOptionalSkill(const std::string& skill);
    const std::unordered_set<std::string>& getRequiredSkills() const;
    const std::unordered_set<std::string>& getOptionalSkills() const;

    // Utility
    static std::string jobTypeToString(JobType type);
    static JobType stringToJobType(const std::string& typeStr);
    void displayJobDetails() const;

    /* ===== CSV Persistence ===== */
    std::string toCSV() const;
    void fromCSV(const std::string& csvLine);

    /* ===== File Handling ===== */
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

#endif
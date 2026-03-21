#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <unordered_map>
#include <vector>
#include <string>

/* ===== Forward Declarations ===== */
class User;
class Candidate;
class Employer;
class Admin;
class Job;
class Application;
class MatchingEngine;

/*
 SystemManager
 - Central controller of the application
 - Owns system-wide data
 - Coordinates authentication, matching, applications
*/

class SystemManager {
private:
    /* ===== User Storage ===== */
    std::unordered_map<std::string, Candidate*> candidates;
    std::unordered_map<std::string, Employer*> employers;
    std::unordered_map<std::string, Admin*> admins;

    /* ===== Job & Application Storage ===== */
    std::unordered_map<int, Job*> jobs;
    std::vector<Application*> applications;

    /* ===== ID Counters ===== */
    int nextUserId;
    int nextJobId;
    int nextApplicationId;

    /* ===== Singleton ===== */
    SystemManager();

public:
    /* ===== Singleton Access ===== */
    static SystemManager& getInstance();

    /* ===== Main Flow ===== */
    void mainMenu();

    /* ===== Authentication ===== */
    void registerUser();
    User* loginUser();

    /* ===== Job Management ===== */
    void addJob(Job* job);
    void approveJob(int jobId);
    const std::unordered_map<int, Job*>& getAllJobs() const;

    /* ===== Application Management ===== */
    void submitApplication(int candidateId, int jobId, double matchScore);
    std::vector<Application*> getApplicationsForJob(int jobId) const;

    /* ===== Matching Coordination ===== */
    std::vector<int> getTopMatchingJobs(
        const Candidate& candidate,
        int topN = 5
    );

    std::vector<std::string> getSkillGap(
        const Candidate& candidate,
        int jobId
    ) const;

    /* ===== File Handling ===== */
    void loadData();
    void saveData();

    /* ===== ID Generation ===== */
    int generateUserId();
    int generateJobId();
    int generateApplicationId();
};

#endif
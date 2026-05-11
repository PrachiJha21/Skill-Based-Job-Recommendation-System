#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "user.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

enum class SkillLevel {
    Beginner,
    Intermediate,
    Expert
};

class Candidate : public User {
private:
    std::unordered_map<std::string, SkillLevel> skills;  // skill -> level
    std::string resumeText;
    std::vector<int> appliedJobIDs;
    double profileStrengthScore;

    // Profile information
    std::vector<std::string> interests;
    std::vector<std::string> experience;  // work experience entries
    std::vector<std::string> education;   // education entries

public:
    Candidate(
        int id = 0,
        std::string username = "",
        std::string password = "",
        std::string email = ""
    );

    /* ===== Skill Management ===== */
    void addSkill(const std::string& skill, SkillLevel level);
    void removeSkill(const std::string& skill);
    void updateSkillLevel(const std::string& skill, SkillLevel level);

    // Getters
    const std::unordered_map<std::string, SkillLevel>& getSkills() const;
    const std::vector<int>& getAppliedJobIDs() const;
    const std::vector<std::string>& getInterests() const;
    const std::vector<std::string>& getExperience() const;
    const std::vector<std::string>& getEducation() const;

    /* ===== Profile Building ===== */
    void buildProfile();
    void addInterest(const std::string& interest);
    void addExperience(const std::string& exp);
    void addEducation(const std::string& edu);

    /* ===== Matching & Recommendation ===== */
    double calculateMatch(int jobId) const;
    std::vector<int> getTopMatches() const;

    /* ===== Job Application ===== */
    void applyForJob(int jobId);

    /* ===== Profile Strength ===== */
    void updateProfileStrength();
    double getProfileStrength() const;

    /* ===== Skill Gap ===== */
    void viewSkillGap(int jobId) const;

    /* ===== Menu ===== */
    
    void displayMenu() override;
    

    /* ===== CSV Persistence ===== */
    std::string toCSV() const;
    void fromCSV(const std::string& csvLine);

    /* ===== File Handling ===== */
    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;
};

#endif
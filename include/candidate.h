#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "user.h"
#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>

class Candidate : public User {
private:
    std::unordered_set<std::string> skills;
    std::string resumeText;
    std::vector<int> appliedJobIDs;
    double profileStrengthScore;

public:
    Candidate(
        int id = 0,
        std::string username = "",
        std::string password = "",
        std::string email = ""
    );

    /* ===== Skill Management ===== */
    void addSkill(const std::string& skill);
    void removeSkill(const std::string& skill);

    const std::unordered_set<std::string>& getSkills() const;

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

    /* ===== File Handling ===== */
    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;
};

#endif
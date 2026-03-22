#include "candidate.h"
#include <iostream>
#include <algorithm>

//Constructor
Candidate::Candidate(
    int id,
    std::string username,
    std::string password,
    std::string email
) : User(id, username, password, "candidate", email), profileStrengthScore(0.0) {}

//Skill Management

void Candidate::addSkill(const std::string& skill) {
    skills.insert(skill);
    updateProfileStrength();
}

const std::unordered_set<std::string>& Candidate::getSkills() const {
    return skills;
}

//Matching and Recommendation

// Placeholder implementations

double Candidate::calculateMatch(int /*jobId*/) const {
    //Placeholder: MatchingEngine should supplu real ranked Jobs
    return {};
}

//  Job Application

void Candidate::applyForJob(int jobId) {
    if (std::find(appliedJobIDs.begin(), appliedJobIDs.end(), jobId) == appliedJobIDs.end()) {
        appliedJobIDs.push_back(jobId);
        std::cout << "Applied for job ID: " << jobId << std::endl;
    } else {
        std::cout << "Already applied for job ID: " << jobId << std::endl;
    }
}

// Profile Strength

void Candidate::updateProfileStrength() {
    //Simple logic: More skills = higher strength
    profileStrengthScore = static_cast<double>(skills.size());
}

double Candidate::getProfileStrength() const {
    return profileStrengthScore;
}

// Skill Gap

void Candidate::viewSkillGap(int jobId) const {
    std::cout << "Skill gap for job ID: " << jobId << "is not implemented yet. \n";
}

// Menu

void Candidate::displayMenu() {
    std::cout << "\n === Candidate Menu === \n";
    std::cout << "1. Add Skill\n";
    std::cout << "2. Remove Skill\n";
    std::cout << "3. View Profile Strength\n";
    std::cout << "4. Apply for Job\n";
    std::cout << "5. Logout\n";
}

// File Handling

void Candidate::saveToFile(std::ofstream& out) const {
    User::saveToFile(out); //Save base User data

    //Save skills
    out << skills.size() << "\n";
    for (const auto& skill : skills) {
        out << skill << "\n";
    }

    //Save profile strength
    out << profileStrengthScore << "\n";
}


void Candidate::loadFromFile(std::ifstream& in) {
    // Load base User data
    User::loadFromFile(in);

    // Load skills
    size_t skillCount;
    in >> skillCount;
    skills.clear();
    for (size_t i = 0; i < skillCount; ++i) {
        std::string skill;
        in >> skill;
        skills.insert(skill);
    }

    // Load applied jobs
    size_t jobCount;
    in >> jobCount;
    appliedJobIDs.clear();
    for (size_t i = 0; i < jobCount; ++i) {
        int jobId;
        in >> jobId;
        appliedJobIDs.push_back(jobId);
    }

    
// Load profile strength
    in >> profileStrengthScore;
}

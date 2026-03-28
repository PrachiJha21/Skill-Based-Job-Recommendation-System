#include "candidate.h"
#include "systemManager.h"
#include "matchingengine.h"
#include "job.h"

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

/* =========================================================
   Constructor
   ========================================================= */

Candidate::Candidate(
    int id,
    string username,
    string password,
    string email
)
    : User(id, username, password, "candidate", email),
      profileStrengthScore(0.0) {}

/* =========================================================
   Helper functions
   ========================================================= */

static std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

static std::string joinString(const std::vector<std::string>& items, char delimiter) {
    std::string result;
    for (size_t i = 0; i < items.size(); ++i) {
        result += items[i];
        if (i + 1 < items.size())
            result.push_back(delimiter);
    }
    return result;
}

static std::string skillLevelToString(SkillLevel level) {
    switch (level) {
        case SkillLevel::Beginner: return "Beginner";
        case SkillLevel::Intermediate: return "Intermediate";
        case SkillLevel::Expert: return "Expert";
        default: return "Beginner";
    }
}

static SkillLevel stringToSkillLevel(const std::string& levelStr) {
    if (levelStr == "Intermediate") return SkillLevel::Intermediate;
    if (levelStr == "Expert") return SkillLevel::Expert;
    return SkillLevel::Beginner;
}

/* =========================================================
   Skill Management
   ========================================================= */

void Candidate::addSkill(const string& skill, SkillLevel level) {
    skills[skill] = level;
    updateProfileStrength();
    cout << "Skill '" << skill << "' added with level: " << skillLevelToString(level) << "\n";
}

void Candidate::removeSkill(const string& skill) {
    if (skills.erase(skill)) {
        updateProfileStrength();
        cout << "Skill removed successfully.\n";
    } else {
        cout << "Skill not found.\n";
    }
}

void Candidate::updateSkillLevel(const string& skill, SkillLevel level) {
    if (skills.count(skill)) {
        skills[skill] = level;
        updateProfileStrength();
        cout << "Skill level updated to: " << skillLevelToString(level) << "\n";
    } else {
        cout << "Skill not found. Add it first.\n";
    }
}

const unordered_map<string, SkillLevel>& Candidate::getSkills() const {
    return skills;
}

const vector<int>& Candidate::getAppliedJobIDs() const {
    return appliedJobIDs;
}

const vector<string>& Candidate::getInterests() const {
    return interests;
}

const vector<string>& Candidate::getExperience() const {
    return experience;
}

const vector<string>& Candidate::getEducation() const {
    return education;
}

/* =========================================================
   Profile Building
   ========================================================= */

void Candidate::buildProfile() {
    int choice;
    do {
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');   
        cout << setw(38) <<" " << "Build Your Profile" << endl;
        cout << setfill('=') << setw(100) << "=" << endl; 
        cout << "1. Add Interest\n";
        cout << "2. Add Work Experience\n";
        cout << "3. Add Education\n";
        cout << "4. View Current Profile\n";
        cout << "5. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string interest;
            cout << "Enter your interest: ";
            getline(cin >> ws, interest);
            addInterest(interest);
            break;
        }
        case 2: {
            string exp;
            cout << "Enter work experience (e.g., 'Software Engineer at XYZ Corp, 2020-2023'): ";
            getline(cin >> ws, exp);
            addExperience(exp);
            break;
        }
        case 3: {
            string edu;
            cout << "Enter education (e.g., 'Bachelor of Computer Science, University ABC, 2016-2020'): ";
            getline(cin >> ws, edu);
            addEducation(edu);
            break;
        }
        case 4: {
            cout << setfill('=') << setw(100) << "=" << endl;
            cout << setfill(' ');   
            cout << setw(38) <<" " << "Your Profile" << endl;
            cout << setfill('=') << setw(100) << "=" << endl;
            cout << setfill(' ');
            cout << "Username: " << getUsername() << "\n"; 
            cout << "Interests: ";
            for (const auto& interest : interests) {
                cout << interest << "\n ";
            }
            cout << setfill('-') << setw(100) << "-" << endl;
            cout << "Work Experience:\n";
            for (const auto& exp : experience) {
                cout << "- " << exp << "\n";
            }
            cout << setfill('-') << setw(100) << "-" << endl;
            cout << "Education:\n";
            for (const auto& edu : education) {
                cout << "- " << edu << "\n";
            }
            cout << setfill('-') << setw(100) << "-" << endl;
            cout << setfill(' ');
            cout << "Skills:\n";
            for (const auto& pair : skills) {
                cout << "- " << pair.first << " (" << skillLevelToString(pair.second) << ")\n";
            }
            break;
        }
        case 5:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

void Candidate::addInterest(const string& interest) {
    interests.push_back(interest);
    cout << "Interest added successfully.\n";
}

void Candidate::addExperience(const string& exp) {
    experience.push_back(exp);
    cout << "Work experience added successfully.\n";
}

void Candidate::addEducation(const string& edu) {
    education.push_back(edu);
    cout << "Education added successfully.\n";
}

/* =========================================================
   Profile Strength
   ========================================================= */

double Candidate::calculateMatch(int /*jobId*/) const {
    //Placeholder: MatchingEngine should supplu real ranked Jobs
    return {};
}
void Candidate::updateProfileStrength() {
    profileStrengthScore = static_cast<double>(skills.size()) * 15.0;  // Increased weight for skills with levels
    if (!experience.empty()) profileStrengthScore += 20.0;
    if (!education.empty()) profileStrengthScore += 20.0;
    if (!interests.empty()) profileStrengthScore += 5.0;
}

double Candidate::getProfileStrength() const {
    return profileStrengthScore;
}

/* =========================================================
   Job Application
   ========================================================= */

void Candidate::applyForJob(int jobId) {
    // Check if job exists
    Job* job = SystemManager::getInstance().getJobById(jobId);
    if (!job) {
        cout << "Job with ID " << jobId << " does not exist.\n";
        return;
    }

    // Check if already applied
    for (int appliedId : appliedJobIDs) {
        if (appliedId == jobId) {
            cout << "You have already applied for this job.\n";
            return;
        }
    }

    // Get screening questions and collect answers
    const auto& questions = job->getScreeningQuestions();
    std::vector<std::string> answers;

    if (!questions.empty()) {
        cout << "\n=== Screening Questions ===\n";
        cout << "Please answer the following questions:\n\n";

        for (size_t i = 0; i < questions.size(); ++i) {
            cout << (i + 1) << ". " << questions[i] << "\n";
            cout << "Your answer: ";
            std::string answer;
            std::getline(std::cin >> std::ws, answer);
            answers.push_back(answer);
            cout << "\n";
        }
    }

    SystemManager::getInstance().submitApplication(
        getID(), jobId, 0.0, answers
    );
    appliedJobIDs.push_back(jobId);
}

/* =========================================================
   Skill Gap
   ========================================================= */

void Candidate::viewSkillGap(int jobId) const {
    auto gaps = SystemManager::getInstance().getSkillGap(*this, jobId);

    if (gaps.empty()) {
        cout << "No skill gap. You match this job well.\n";
        return;
    }

    cout << "Missing skills:\n";
    for (const string& skill : gaps) {
        cout << "- " << skill << endl;
    }
}

/* =========================================================
   Candidate Menu (SESSION LOOP)
   ========================================================= */

void Candidate::displayMenu() {
    int choice;

    do {
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');   // reset fill character
        cout << setw(40) <<" " << "Candidate Dashboard" << endl;
        cout << setfill('=') << setw(100) << "=" << endl;
        cout << setfill(' ');
        cout << "1. Add/Update Skill with Level\n";
        cout << "2. Remove Skill\n";
        cout << "3. Build Profile (Interests/Experience/Education)\n";
        cout << "4. View Profile Strength\n";
        cout << "5. View Top Matching Jobs\n";
        cout << "6. View All Jobs\n";
        cout << "7. View Skill Gap for Job\n";
        cout << "8. Apply for Job\n";
        cout << "9. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string skill;
            int levelChoice;
            cout << "Enter skill: ";
            getline(cin >> ws, skill);
            
            cout << "Select skill level:\n";
            cout << "1. Beginner\n";
            cout << "2. Intermediate\n";
            cout << "3. Expert\n";
            cout << "Choice: ";
            cin >> levelChoice;
            
            SkillLevel level = SkillLevel::Beginner;
            if (levelChoice == 2) level = SkillLevel::Intermediate;
            else if (levelChoice == 3) level = SkillLevel::Expert;
            
            if (skills.count(skill)) {
                updateSkillLevel(skill, level);
            } else {
                addSkill(skill, level);
            }
            break;
        }

        case 2: {
            string skill;
            cout << "Enter skill to remove: ";
            getline(cin >> ws, skill);
            removeSkill(skill);
            break;
        }

        case 3:
            buildProfile();
            break;

        case 4:
            cout << "Profile Strength: "
                 << getProfileStrength() << "%\n";
            break;

        case 5: {
            auto matches =
                SystemManager::getInstance().getTopMatchingJobsWithScores(*this);

            if (matches.empty()) {
                cout << "No matching jobs found.\n";
            } else {
                cout << "\n=== Top Matching Jobs ===\n";
                for (const auto& match : matches) {
                    int jobId = match.first;
                    double score = match.second;
                    Job* job = SystemManager::getInstance().getJobById(jobId);
                    if (job) {
                        cout << "Match Score: " << fixed << setprecision(1) << score << "%\n";
                        job->displayJobDetails();
                        cout << "----------------------------------------\n";
                    }
                }
            }
            break;
        }

        case 6: {
            auto allJobs = SystemManager::getInstance().getAllJobs();
            if (allJobs.empty()) {
                cout << "No jobs available.\n";
            } else {
                cout << "\n=== All Available Jobs ===\n";
                for (const auto& pair : allJobs) {
                    pair.second->displayJobDetails();
                    cout << "----------------------------------------\n";
                }
            }
            break;
        }

        case 7: {
            int jobId;
            cout << "Enter Job ID: ";
            cin >> jobId;
            viewSkillGap(jobId);
            break;
        }

        case 8: {
            int jobId;
            cout << "Enter Job ID to apply: ";
            cin >> jobId;
            applyForJob(jobId);
            break;
        }

        case 9:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 9);  // ✅ SESSION ENDS ONLY ON LOGOUT
}

/* =========================================================
   CSV Persistence
   ========================================================= */

std::string Candidate::toCSV() const {
    // Skills with levels: skill1:level1|skill2:level2
    std::vector<std::string> skillItems;
    for (const auto& pair : skills) {
        skillItems.push_back(pair.first + ":" + skillLevelToString(pair.second));
    }
    std::string skillCsv = joinString(skillItems, '|');

    // Applied jobs
    std::vector<std::string> appliedList;
    for (int id : appliedJobIDs) {
        appliedList.push_back(std::to_string(id));
    }
    std::string appliedCsv = joinString(appliedList, '|');

    // Profile information
    std::string interestsCsv = joinString(interests, '|');
    std::string experienceCsv = joinString(experience, '|');
    std::string educationCsv = joinString(education, '|');

    return std::to_string(id) + "," + username + "," + std::to_string(passwordHash) + "," + role + "," + email + "," + 
           skillCsv + "," + resumeText + "," + appliedCsv + "," + std::to_string(profileStrengthScore) + "," +
           interestsCsv + "," + experienceCsv + "," + educationCsv;
}

void Candidate::fromCSV(const std::string& csvLine) {
    auto fields = splitString(csvLine, ',');

    if (fields.size() < 13) {
        return;
    }

    id = std::stoi(fields[0]);
    username = fields[1];
    passwordHash = static_cast<size_t>(std::stoull(fields[2]));
    role = fields[3];
    email = fields[4];

    // Parse skills with levels
    skills.clear();
    if (!fields[5].empty()) {
        auto skillTokens = splitString(fields[5], '|');
        for (const auto& token : skillTokens) {
            size_t colonPos = token.find(':');
            if (colonPos != std::string::npos) {
                std::string skillName = token.substr(0, colonPos);
                std::string levelStr = token.substr(colonPos + 1);
                skills[skillName] = stringToSkillLevel(levelStr);
            }
        }
    }

    resumeText = fields[6];

    // Applied jobs
    appliedJobIDs.clear();
    if (!fields[7].empty()) {
        auto appliedTokens = splitString(fields[7], '|');
        for (const auto& a : appliedTokens) {
            if (!a.empty()) {
                appliedJobIDs.push_back(std::stoi(a));
            }
        }
    }

    profileStrengthScore = std::stod(fields[8]);

    // Profile information
    interests.clear();
    if (!fields[9].empty()) {
        interests = splitString(fields[9], '|');
    }

    experience.clear();
    if (!fields[10].empty()) {
        experience = splitString(fields[10], '|');
    }

    education.clear();
    if (!fields[11].empty()) {
        education = splitString(fields[11], '|');
    }
}

/* =========================================================
   File Handling
   ========================================================= */

void Candidate::saveToFile(ofstream& out) const {
    out << toCSV() << "\n";
}

void Candidate::loadFromFile(ifstream& in) {
    std::string line;
    std::getline(in, line);
    if (line.empty())
        return;
    fromCSV(line);
}


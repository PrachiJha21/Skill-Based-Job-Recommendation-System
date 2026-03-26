#ifndef JOB_H
#define JOB_H

#include <string>
#include <unordered_set>
#include <fstream>

class Job {
private:
    int id;
    std::string title;
    std::unordered_set<std::string> requiredSkills;
    std::unordered_set<std::string> optionalSkills;

public:
    Job(
        int id = 0,
        std::string title = ""
    );
    // Getters
    int getID() const;
    std::string getTitle() const;
    //const std::unordered_set<std::string>& getRequiredSkills() const;

    /// Additional setters and helpers
    void setTitle(const std::string& newTitle);
    void clearSkills();


    /* ===== Skill Requirements ===== */
    void addRequiredSkill(const std::string& skill);
    const std::unordered_set<std::string>& getRequiredSkills() const;
    
    const std::unordered_set<std::string>& getOptionalSkills() const;
    void addOptionalSkill(const std::string& skill);    

    /* ===== File Handling ===== */
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

#endif
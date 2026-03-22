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

public:
    Job(
        int id = 0,
        std::string title = ""
    );

    int getID() const;
    std::string getTitle() const;

    /* ===== Skill Requirements ===== */
    void addRequiredSkill(const std::string& skill);
    const std::unordered_set<std::string>& getRequiredSkills() const;

    /* ===== File Handling ===== */
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

#endif
#include "job.h"

/* =========================
   Constructor
   ========================= */

Job::Job(
    int id,
    std::string title
)
    : id(id),
      title(title) {}

/* =========================
   Getters
   ========================= */

int Job::getID() const {
    return id;
}

std::string Job::getTitle() const {
    return title;
}

void Job::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Job::clearSkills() {
    requiredSkills.clear();
}


/* =========================
   Skill Requirements
   ========================= */

void Job::addRequiredSkill(const std::string& skill) {
    if (!skill.empty()) {
        requiredSkills.insert(skill);
    }
}

const std::unordered_set<std::string>& Job::getRequiredSkills() const {
    return requiredSkills;
}

const std::unordered_set<std::string>& Job::getOptionalSkills() const {
    return optionalSkills;
}

void Job::addOptionalSkill(const std::string& skill) {
    if (!skill.empty()) {
        optionalSkills.insert(skill);
    }
}

/* =========================
   File Handling
   ========================= */

void Job::saveToFile(std::ofstream& out) const {
    out << id << "\n";
    out << title << "\n";

    out << requiredSkills.size() << "\n";
    for (const auto& skill : requiredSkills) {
        out << skill << "\n";
    }
}

void Job::loadFromFile(std::ifstream& in) {
    if (!(in >> id)) {
        return;
    }

    in.ignore(100000, '\n');
    std::getline(in, title);

    size_t skillCount = 0;
    in >> skillCount;
    in.ignore(100000, '\n');

    requiredSkills.clear();
    for (size_t i = 0; i < skillCount; ++i) {
        std::string skill;
        std::getline(in, skill);

        if (!skill.empty()) {
            requiredSkills.insert(skill);
        }
    }
}

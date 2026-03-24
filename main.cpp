#include <iostream>

#include "systemManager.h"
#include "candidate.h"
#include "job.h"
#include "matchingengine.h"

using namespace std;

int main() {
    cout << "=== MODULE TESTING START ===\n";

    // Get system instance
    SystemManager& system = SystemManager::getInstance();

    // ----- Create a Candidate -----
    Candidate* c1 = new Candidate(1, "alice", "pass123", "alice@gmail.com");
    c1->addSkill("C++");
    c1->addSkill("DSA");
    c1->addSkill("OOP");

    // ----- Create Jobs -----
    Job* j1 = new Job(1, "Software Engineer");
    j1->addRequiredSkill("C++");
    j1->addRequiredSkill("DSA");

    Job* j2 = new Job(2, "Backend Developer");
    j2->addRequiredSkill("Java");
    j2->addRequiredSkill("SQL");

    // ----- Add jobs to system -----
    system.addJob(j1);
    system.addJob(j2);

    // ----- Run Matching -----
    cout << "\nTop Matching Jobs:\n";
    vector<int> matches = system.getTopMatchingJobs(*c1, 5);

    for (int jobId : matches) {
        cout << "Matched Job ID: " << jobId << endl;
    }

    // ----- Skill Gap Test -----
    cout << "\nSkill Gap for Job 2:\n";
    vector<string> gaps = system.getSkillGap(*c1, 2);

    for (const string& skill : gaps) {
        cout << "Missing: " << skill << endl;
    }

    cout << "\n=== MODULE TESTING END ===\n";
    return 0;
}

#include "employer.h"
#include "systemManager.h"
#include "job.h"
#include "application.h"

#include <iostream>
#include <algorithm>

using namespace std;

/* =========================
   Constructor
   ========================= */

Employer::Employer(
    int id,
    string username,
    string password,
    string email,
    string companyName
)
    : User(id, username, password, "employer", email),
      companyName(companyName) {}


/* =========================
   Post Job
   ========================= */

void Employer::postJob() {
    SystemManager& system = SystemManager::getInstance();

    int jobId = system.generateJobId();
    string title;

    cout << "Enter job title: ";
    getline(cin >> ws, title);

    Job* job = new Job(jobId, title);

    int skillCount;
    cout << "How many required skills? ";
    cin >> skillCount;

    for (int i = 0; i < skillCount; i++) {
        string skill;
        cout << "Enter skill " << i + 1 << ": ";
        getline(cin >> ws, skill);
        job->addRequiredSkill(skill);
    }

    system.addJob(job);
    postedJobIDs.push_back(jobId);

    cout << "Job posted successfully with ID "
         << jobId << ". Awaiting admin approval.\n";
}


/* =========================
   Edit Job
   ========================= */

void Employer::editJob(int jobId) {
    SystemManager& system = SystemManager::getInstance();
    Job* job = system.getJobById(jobId);

    if (!job) {
        cout << "Job not found.\n";
        return;
    }

    string newTitle;
    cout << "Enter new job title: ";
    getline(cin >> ws, newTitle);
    job->setTitle(newTitle);

    cout << "Clear and re‑enter skills? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        job->clearSkills();

        int skillCount;
        cout << "How many required skills? ";
        cin >> skillCount;

        for (int i = 0; i < skillCount; i++) {
            string skill;
            cout << "Enter skill " << i + 1 << ": ";
            getline(cin >> ws, skill);
            job->addRequiredSkill(skill);
        }
    }

    cout << "Job updated successfully.\n";
}


/* =========================
   Delete Job
   ========================= */

void Employer::deleteJob(int jobId) {
    SystemManager& system = SystemManager::getInstance();

    if (!system.removeJob(jobId)) {
        cout << "Job not found.\n";
        return;
    }

    postedJobIDs.erase(
        remove(postedJobIDs.begin(), postedJobIDs.end(), jobId),
        postedJobIDs.end()
    );

    cout << "Job deleted successfully.\n";
}


/* =========================
   View Applicants
   ========================= */

void Employer::viewApplicants(int jobId) const {
    SystemManager& system = SystemManager::getInstance();
    auto apps = system.getApplicationsForJob(jobId);

    if (apps.empty()) {
        cout << "No applications for this job yet.\n";
        return;
    }

    cout << "\nApplicants for Job ID " << jobId << ":\n";

    for (const Application* app : apps) {
        cout << "Candidate ID: " << app->getCandidateId()
             << " | Status: "
             << Application::statusToString(app->getStatus())
             << "\n";
    }
}


/* =========================
   Getters
   ========================= */

string Employer::getCompanyName() const {
    return companyName;
}

const vector<int>& Employer::getPostedJobIDs() const {
    return postedJobIDs;
}


/* =========================
   Menu (SESSION LOOP ✅)
   ========================= */

void Employer::displayMenu() {
    int choice;

    do {
        cout << "\n======== Employer Menu ========\n";
        cout << "1. Post Job\n";
        cout << "2. Edit Job\n";
        cout << "3. Delete Job\n";
        cout << "4. View Applicants\n";
        cout << "5. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            postJob();
            break;

        case 2: {
            int jobId;
            cout << "Enter Job ID to edit: ";
            cin >> jobId;
            editJob(jobId);
            break;
        }

        case 3: {
            int jobId;
            cout << "Enter Job ID to delete: ";
            cin >> jobId;
            deleteJob(jobId);
            break;
        }

        case 4: {
            int jobId;
            cout << "Enter Job ID to view applicants: ";
            cin >> jobId;
            viewApplicants(jobId);
            break;
        }

        case 5:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);
}


/* =========================
   File Handling
   ========================= */

void Employer::saveToFile(ofstream& out) const {
    User::saveToFile(out);

    out << companyName << "\n";
    out << postedJobIDs.size() << "\n";

    for (int id : postedJobIDs)
        out << id << " ";
    out << "\n";
}

void Employer::loadFromFile(ifstream& in) {
    User::loadFromFile(in);

    in >> companyName;

    size_t count;
    in >> count;

    postedJobIDs.clear();
    for (size_t i = 0; i < count; i++) {
        int id;
        in >> id;
        postedJobIDs.push_back(id);
    }
}

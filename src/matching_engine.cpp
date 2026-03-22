#include "matchingengine.h"
#include <algorithm>

/* =========================================================
   MatchingEngine
   Stateless utility-style implementation
   ========================================================= */

/*
 * Calculates a match score between a candidate and a job.
 * Current logic:
 *  - Counts overlapping skills
 *  - Normalizes by total job-required skills
 */
double MatchingEngine::calculateMatchScore(
    const Candidate& candidate,
    const Job& job
) {
    const auto& candidateSkills = candidate.getSkills();
    const auto& jobSkills = job.getRequiredSkills();

    if (jobSkills.empty()) {
        return 0.0;
    }

    int matchedSkills = 0;
    for (const auto& skill : jobSkills) {
        if (candidateSkills.count(skill)) {
            matchedSkills++;
        }
    }

    return static_cast<double>(matchedSkills) /
           static_cast<double>(jobSkills.size());
}

/*
 * Returns the top N matching job IDs for a candidate.
 * Jobs are ranked by match score (descending).
 */
std::vector<int> MatchingEngine::getTopMatchingJobs(
    const Candidate& candidate,
    const std::unordered_map<int, Job>& jobs,
    int topN
) {
    std::vector<std::pair<int, double>> scoredJobs;

    // Calculate score for each job
    for (const auto& [jobId, job] : jobs) {
        double score = calculateMatchScore(candidate, job);
        scoredJobs.emplace_back(jobId, score);
    }

    // Sort jobs by score (highest first)
    std::sort(scoredJobs.begin(), scoredJobs.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    // Extract top N job IDs
    std::vector<int> topJobs;
    for (int i = 0; i < topN && i < static_cast<int>(scoredJobs.size()); ++i) {
        topJobs.push_back(scoredJobs[i].first);
    }

    return topJobs;
}

/*
 * Returns the list of skills the candidate is missing
 * for a given job.
 */
std::vector<std::string> MatchingEngine::getSkillGap(
    const Candidate& candidate,
    const Job& job
) {
    std::vector<std::string> missingSkills;

    const auto& candidateSkills = candidate.getSkills();
    const auto& jobSkills = job.getRequiredSkills();

    for (const auto& skill : jobSkills) {
        if (!candidateSkills.count(skill)) {
            missingSkills.push_back(skill);
        }
    }

    return missingSkills;
}
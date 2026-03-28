#ifndef MATCHINGENGINE_H
#define MATCHINGENGINE_H

#include <vector>
#include <unordered_map>
#include <string>

#include "candidate.h"
#include "job.h"

/*
 MatchingEngine
 - Stateless service class
 - Handles matching, ranking, skill gap analysis
*/

class MatchingEngine {
public:
    static double calculateMatchScore(
        const Candidate& candidate,
        const Job& job
    );

    static std::vector<int> getTopMatchingJobs(
        const Candidate& candidate,
        const std::unordered_map<int, Job>& jobs,
        int topN = 5
    );

    static std::vector<std::pair<int, double>> getTopMatchingJobsWithScores(
        const Candidate& candidate,
        const std::unordered_map<int, Job>& jobs,
        int topN = 5
    );

    static std::vector<std::string> getSkillGap(
        const Candidate& candidate,
        const Job& job
    );
};

#endif
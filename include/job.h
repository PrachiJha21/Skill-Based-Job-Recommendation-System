#ifndef JOB_H
#define JOB_H

#include <cstdint>

// Forward declaration to avoid circular dependency
class MatchingEngine;

/**
 * @brief Abstract base class representing a unit of work
 *        that can be executed by the MatchingEngine.
 */
class Job {
public:
    // Each job has a unique id (useful for logging / debugging)
    explicit Job(std::uint64_t id);
    virtual ~Job() = default;

    // Prevent copying (jobs are usually owned/executed once)
    Job(const Job&) = delete;
    Job& operator=(const Job&) = delete;

    // Allow moving if needed
    Job(Job&&) = default;
    Job& operator=(Job&&) = default;

    /**
     * @brief Execute the job on the matching engine
     */
    virtual void execute(MatchingEngine& engine) = 0;

    std::uint64_t id() const;

protected:
    std::uint64_t jobId_;
};

#endif // JOB_H
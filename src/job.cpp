#include "job.h"

Job::Job(std::uint64_t id)
    : jobId_(id) {}

std::uint64_t Job::id() const {
    return jobId_;
}
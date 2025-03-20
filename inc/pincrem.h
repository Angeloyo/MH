#pragma once
#include <algorithm>
#include <problem.h>

class ProblemIncrem : public Problem {
  size_t size;

public:
  ProblemIncrem(size_t size) : Problem() { this->size = size; }
  tFitness fitness(const tSolution &solution) override;
  tSolution createSolution() override;
  size_t getSolutionSize() override { return size; }
  std::pair<tDomain, tDomain> getSolutionDomainRange() override {
    return std::make_pair(false, true);
  }
};

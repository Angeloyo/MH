#include <algorithm>
#include <pincrem.h>
#include <random.hpp>

tFitness ProblemIncrem::fitness(const tSolution &solution) {
  tFitness count = 0;

  for (int i = 0; i < solution.size(); i++) {
    if (solution[i]) {
      if (i % 2 == 0) {
        count += 1;
      } else {
        count -= 1;
      }
    }
  }
  return count;
}

tSolution ProblemIncrem::createSolution() {
  tSolution solution(size);
  for (int i = 0; i < solution.size(); i++) {
    solution[i] = Random::get<bool>();
  }
  return solution;
}

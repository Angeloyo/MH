#pragma once
#include <vector>
/**
 * Typedef for the fitness function return type.
 * For MDD, this represents the differential dispersion value to minimize.
 */
typedef float tFitness;

/**
 * Domain of each element. For MDD:
 * - true: element is selected in the solution M
 * - false: element is not selected
 */
typedef bool tDomain;

/**
 * Represents an option (element index) for the heuristic algorithms.
 */
typedef int tOption;

/**
 * Represents the heuristic value for element selection.
 * In MDD, this indicates how much an element affects the dispersion.
 */
typedef tFitness tHeuristic;

/**
 * Solution class to represent a solution to the MDD problem.
 * A binary vector where true means the element is selected.
 * For the MDD problem, exactly m elements must be true.
 */
typedef std::vector<tDomain> tSolution;

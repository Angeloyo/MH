#pragma once
#include <problem.h>
#include <string>
#include <vector>

/**
 * Class that represents factorized information for the MDD problem.
 * This stores the sum of distances of each selected element to all other selected elements.
 */
class MDDSolutionInfo : public SolutionFactoringInfo {
public:
    // Sum of distances for each selected element to all other selected elements
    std::vector<float> sumDistances;
    // Indices of selected elements
    std::vector<int> selected;
    // Indices of non-selected elements
    std::vector<int> nonSelected;

    // Constructor
    MDDSolutionInfo() {}
    
    // Destructor
    virtual ~MDDSolutionInfo() override = default;
};

/**
 * Implementation of the MDD (Minimum Differential Dispersion) problem.
 * 
 * The goal is to select a subset M of m elements from a set N of n elements
 * to minimize the differential dispersion, which is defined as:
 * 
 * max(sum_distances) - min(sum_distances)
 * 
 * where sum_distances is the sum of distances from one selected element to all
 * other selected elements.
 */
class ProblemMDD : public Problem {
private:
    // Number of elements in the original set
    int n;
    // Number of elements to select
    int m;
    // Matrix of distances between elements
    std::vector<std::vector<float>> distances;
    // Name of the instance
    std::string instanceName;

public:
    /**
     * Constructor that loads a problem instance from a file.
     * 
     * @param filename The path to the file containing the instance data
     */
    ProblemMDD(const std::string& filename);

    /**
     * Evaluates a solution by calculating the differential dispersion.
     * 
     * @param solution The solution to evaluate
     * @return The differential dispersion value (to be minimized)
     */
    tFitness fitness(const tSolution& solution) override;

    /**
     * Factorized fitness calculation for local search.
     * 
     * @param solution Current solution
     * @param solution_info Factorized information
     * @param pos_change Position to change (remove element)
     * @param new_value New value (add element)
     * @return New fitness after the change
     */
    tFitness fitness(const tSolution& solution,
                     SolutionFactoringInfo* solution_info,
                     unsigned pos_change, tDomain new_value) override;

    /**
     * Generates solution factoring information for efficient local search.
     * 
     * @param solution The solution to generate info for
     * @return Pointer to the factoring information
     */
    SolutionFactoringInfo* generateFactoringInfo(const tSolution& solution) override;

    /**
     * Updates factoring information after a movement is applied.
     * 
     * @param solution_info The info to update
     * @param solution The solution after movement
     * @param pos_change The position changed
     * @param new_value The new value
     */
    void updateSolutionFactoringInfo(SolutionFactoringInfo* solution_info,
                                     const tSolution& solution,
                                     unsigned pos_change,
                                     tDomain new_value) override;

    /**
     * Creates a random valid solution with exactly m elements selected.
     * 
     * @return A valid random solution
     */
    tSolution createSolution() override;

    /**
     * Returns the size of the solution (n elements).
     * 
     * @return The solution size
     */
    size_t getSolutionSize() override { return n; }

    /**
     * Returns the range of domain values.
     * 
     * @return Pair of (min, max) domain values
     */
    std::pair<tDomain, tDomain> getSolutionDomainRange() override {
        return std::make_pair(false, true);
    }

    /**
     * Returns the distance between two elements.
     * 
     * @param i First element index
     * @param j Second element index
     * @return The distance between elements i and j
     */
    float getDistance(int i, int j) const;

    /**
     * Returns the number of elements in the original set.
     * 
     * @return The number of elements n
     */
    int getN() const { return n; }

    /**
     * Returns the number of elements to select.
     * 
     * @return The number of elements to select m
     */
    int getM() const { return m; }

    /**
     * Returns the name of the instance.
     * 
     * @return The instance name
     */
    std::string getInstanceName() const { return instanceName; }
}; 
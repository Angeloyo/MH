#pragma once
#include <mh.h>
#include <timer.h>
#include <vector>

/**
 * Implementation of the Greedy algorithm for the MDD problem.
 * 
 * Starts with a random element and selects m-1 more based on 
 * which elements minimize the dispersion with the already selected ones.
 */
class GreedyMDD : public MH {
public:
    /**
     * Constructor.
     */
    GreedyMDD() : MH() {}
    
    /**
     * Destructor.
     */
    virtual ~GreedyMDD() {}
    
    /**
     * Run the Greedy algorithm.
     * 
     * @param problem The MDD problem to solve
     * @param maxevals Maximum number of evaluations (ignored in greedy)
     * @return A ResultMH containing the solution found, its fitness, and the number of evaluations
     */
    ResultMH optimize(Problem* problem, int maxevals) override;
    
    /**
     * Get the name of the algorithm.
     * 
     * @return The algorithm name
     */
    std::string getName() const { return "GreedyMDD"; }
}; 
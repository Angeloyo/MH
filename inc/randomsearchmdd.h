#pragma once
#include <mh.h>
#include <timer.h>

/**
 * Implementation of the Random Search for the MDD problem.
 * 
 * Generates 100,000 random solutions and returns the best one found.
 */
class RandomSearchMDD : public MH {
public:
    /**
     * Constructor.
     */
    RandomSearchMDD() : MH() {}
    
    /**
     * Destructor.
     */
    virtual ~RandomSearchMDD() {}
    
    /**
     * Generate 100,000 random solutions and return the best one.
     * 
     * @param problem The MDD problem to solve
     * @param maxevals Maximum number of evaluations (ignored, always uses 100,000)
     * @return A ResultMH containing the best solution found, its fitness, and the number of evaluations
     */
    ResultMH optimize(Problem* problem, int maxevals) override;
    
    /**
     * Get the name of the algorithm.
     * 
     * @return The algorithm name
     */
    std::string getName() const { return "RandomSearchMDD"; }
}; 
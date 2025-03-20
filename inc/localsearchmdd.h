#pragma once
#include <mh.h>
#include <timer.h>
#include <vector>
#include <string>

/**
 * Tipo de exploración del entorno para la búsqueda local
 */
enum class ExplorationStrategy {
    RANDOM,  // Exploración en orden aleatorio (randLS)
    HEURISTIC // Exploración basada en heurística (heurLS)
};

/**
 * Implementation of the Local Search algorithm for the MDD problem.
 * 
 * Uses the first-improvement strategy and Int(Sel,i,j) move that swaps
 * a selected element i with a non-selected element j.
 * Stops when no improvement is found in the entire neighborhood or when
 * the maximum number of evaluations is reached.
 */
class LocalSearchMDD : public MH {
private:
    ExplorationStrategy strategy; // Estrategia de exploración
    
public:
    /**
     * Constructor.
     * 
     * @param strategy The exploration strategy to use (RANDOM or HEURISTIC)
     */
    LocalSearchMDD(ExplorationStrategy strategy) : MH(), strategy(strategy) {}
    
    /**
     * Destructor.
     */
    virtual ~LocalSearchMDD() {}
    
    /**
     * Run the Local Search algorithm.
     * 
     * @param problem The MDD problem to solve
     * @param maxevals Maximum number of evaluations (stops at 100,000 or when no improvement is found)
     * @return A ResultMH containing the best solution found, its fitness, and the number of evaluations
     */
    ResultMH optimize(Problem* problem, int maxevals) override;
    
    /**
     * Get the name of the algorithm.
     * 
     * @return The algorithm name, including the exploration strategy
     */
    std::string getName() const {
        if (strategy == ExplorationStrategy::RANDOM) {
            return "randLS";
        } else {
            return "heurLS";
        }
    }
}; 
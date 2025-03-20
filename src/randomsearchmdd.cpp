#include <randomsearchmdd.h>
#include <cassert>
#include <iostream>
#include <iomanip>

/**
 * Generate 100,000 random solutions and return the best one.
 * 
 * @param problem The MDD problem to solve
 * @param maxevals Maximum number of evaluations (ignored, always uses 100,000)
 * @return A ResultMH containing the best solution found, its fitness, and the number of evaluations
 */
ResultMH RandomSearchMDD::optimize(Problem* problem, int maxevals) {
    // Always use 100,000 evaluations as per the problem specification
    const int NUM_EVALUATIONS = 100000;
    
    // Initialize timers and counters
    Timer timer;
    timer.start();
    
    // Variables to store the best solution
    tSolution best_solution;
    tFitness best_fitness = std::numeric_limits<tFitness>::max(); // Initialize to worst possible
    
    // Generate and evaluate 100,000 random solutions
    for (int i = 0; i < NUM_EVALUATIONS; i++) {
        // Create a random solution
        tSolution solution = problem->createSolution();
        
        // Evaluate it
        tFitness fitness = problem->fitness(solution);
        
        // Update best if this solution is better
        if (fitness < best_fitness) {
            best_solution = solution;
            best_fitness = fitness;
        }
        
        // Optional: Print progress every 10,000 evaluations
        if ((i + 1) % 10000 == 0) {
            std::cout << "RandomSearchMDD: " << (i + 1) << " evaluations completed. "
                      << "Best fitness so far: " << best_fitness << std::endl;
        }
    }
    
    // Stop the timer
    timer.stop();
    
    // Print summary
    std::cout << "\nRandomSearchMDD completed in " << std::fixed << std::setprecision(2)
              << timer.elapsed() << " seconds." << std::endl;
    std::cout << "Best fitness found: " << best_fitness << std::endl;
    
    // Return the result
    return ResultMH(best_solution, best_fitness, NUM_EVALUATIONS);
} 
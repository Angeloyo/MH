#include <problemmdd.h>
#include <localsearchmdd.h>
#include <timer.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <random.hpp>

// Helper function to print a solution
void printSolution(const tSolution& solution, const std::string& title) {
    std::cout << title << ": [";
    bool first = true;
    for (size_t i = 0; i < solution.size(); i++) {
        if (solution[i]) {
            if (!first) std::cout << ", ";
            std::cout << i;
            first = false;
        }
    }
    std::cout << "]" << std::endl;
}

// Main function for testing LocalSearch algorithms
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <path_to_instance_file> <seed> <strategy>" << std::endl;
        std::cout << "  strategy: 0 for randLS (random), 1 for heurLS (heuristic)" << std::endl;
        return 1;
    }
    
    try {
        // Get command line arguments
        std::string instance_path = argv[1];
        long seed = std::stol(argv[2]);
        int strategyInt = std::stoi(argv[3]);
        
        // Validate strategy
        if (strategyInt < 0 || strategyInt > 1) {
            std::cerr << "Error: strategy must be 0 (random) or 1 (heuristic)" << std::endl;
            return 1;
        }
        
        // Convert to enum
        ExplorationStrategy strategy = (strategyInt == 0) ? 
                                      ExplorationStrategy::RANDOM : 
                                      ExplorationStrategy::HEURISTIC;
        
        // Initialize random number generator with the seed
        Random::seed(seed);
        
        // Load the problem instance
        std::cout << "Loading problem instance from: " << instance_path << std::endl;
        ProblemMDD problem(instance_path);
        
        std::cout << "Instance: " << problem.getInstanceName() << std::endl;
        std::cout << "n = " << problem.getN() << ", m = " << problem.getM() << std::endl;
        
        // Create and run the LocalSearch algorithm
        std::cout << "\nRunning LocalSearch algorithm with " 
                  << (strategy == ExplorationStrategy::RANDOM ? "random" : "heuristic") 
                  << " exploration..." << std::endl;
        
        LocalSearchMDD localSearch(strategy);
        
        // Start timer
        Timer timer;
        timer.start();
        
        // Run the algorithm (100,000 evaluations max as per guión)
        ResultMH result = localSearch.optimize(&problem, 100000);
        
        // Stop timer
        timer.stop();
        
        // Print results
        std::cout << "\nResults:" << std::endl;
        std::cout << "Execution time: " << timer.elapsed() << " seconds" << std::endl;
        std::cout << "Total evaluations: " << result.evaluations << std::endl;
        std::cout << "Best fitness: " << result.fitness << std::endl;
        printSolution(result.solution, "Best solution");
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 
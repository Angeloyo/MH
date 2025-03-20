#include <problemmdd.h>
#include <randomsearchmdd.h>
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

// Main function for testing RandomSearchMDD
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <path_to_instance_file> <seed>" << std::endl;
        return 1;
    }
    
    try {
        // Get command line arguments
        std::string instance_path = argv[1];
        long seed = std::stol(argv[2]);
        
        // Initialize random number generator with the seed
        Random::seed(seed);
        
        // Load the problem instance
        std::cout << "Loading problem instance from: " << instance_path << std::endl;
        ProblemMDD problem(instance_path);
        
        std::cout << "Instance: " << problem.getInstanceName() << std::endl;
        std::cout << "n = " << problem.getN() << ", m = " << problem.getM() << std::endl;
        
        // Create and run the random search algorithm
        std::cout << "\nRunning Random Search algorithm..." << std::endl;
        RandomSearchMDD random_search;
        
        // Start timer
        Timer timer;
        timer.start();
        
        // Run the algorithm with 100,000 evaluations
        ResultMH result = random_search.optimize(&problem, 100000);
        
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
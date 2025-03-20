#include <problemmdd.h>
#include <greedymdd.h>
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

// Main function for testing GreedyMDD
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
        
        // Create and run the greedy algorithm
        std::cout << "\nRunning Greedy algorithm..." << std::endl;
        GreedyMDD greedy;
        
        // Start timer
        Timer timer;
        timer.start();
        
        // Run the algorithm
        ResultMH result = greedy.optimize(&problem, 0); // El parámetro maxevals no se usa en greedy
        
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
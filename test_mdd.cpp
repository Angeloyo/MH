#include <problemmdd.h>
#include <timer.h>
#include <iostream>
#include <iomanip>
#include <vector>

// Helper function to print a solution
void printSolution(const tSolution& solution, const std::string& title) {
    std::cout << title << ": [";
    bool first = true;
    for (int i = 0; i < solution.size(); i++) {
        if (solution[i]) {
            if (!first) std::cout << ", ";
            std::cout << i;
            first = false;
        }
    }
    std::cout << "]" << std::endl;
}

// Helper function to print a vector of floats
void printVector(const std::vector<float>& v, const std::string& title) {
    std::cout << title << ": [";
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << std::fixed << std::setprecision(2) << v[i];
    }
    std::cout << "]" << std::endl;
}

// Main function for testing
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_instance_file>" << std::endl;
        return 1;
    }
    
    try {
        // Load the problem instance
        std::cout << "Loading problem instance from: " << argv[1] << std::endl;
        ProblemMDD problem(argv[1]);
        
        std::cout << "Instance: " << problem.getInstanceName() << std::endl;
        std::cout << "n = " << problem.getN() << ", m = " << problem.getM() << std::endl;
        
        // Create a random solution
        std::cout << "\nCreating a random solution..." << std::endl;
        tSolution solution = problem.createSolution();
        printSolution(solution, "Random solution");
        
        // Evaluate the solution
        Timer timer;
        timer.start();
        tFitness fitness = problem.fitness(solution);
        timer.stop();
        
        std::cout << "Fitness: " << fitness << std::endl;
        std::cout << "Evaluation time: " << timer.elapsed() * 1000 << " ms" << std::endl;
        
        // Test factorization
        std::cout << "\nTesting factorization..." << std::endl;
        
        // Generate factoring info
        timer.reset();
        timer.start();
        SolutionFactoringInfo* info = problem.generateFactoringInfo(solution);
        timer.stop();
        MDDSolutionInfo* mddInfo = dynamic_cast<MDDSolutionInfo*>(info);
        
        std::cout << "Factoring info generation time: " << timer.elapsed() * 1000 << " ms" << std::endl;
        
        if (mddInfo) {
            std::cout << "Selected elements: " << mddInfo->selected.size() << std::endl;
            std::cout << "Non-selected elements: " << mddInfo->nonSelected.size() << std::endl;
            printVector(mddInfo->sumDistances, "Sum distances");
            
            // Test the swap move if there are enough elements
            if (!mddInfo->selected.empty() && !mddInfo->nonSelected.empty()) {
                int selectedElem = mddInfo->selected[0];
                unsigned nonSelectedIdx = 0;
                
                std::cout << "\nTesting swap move: swapping selected element " 
                          << selectedElem << " with non-selected element " 
                          << mddInfo->nonSelected[nonSelectedIdx] << std::endl;
                
                // Calculate new fitness with factorization
                timer.reset();
                timer.start();
                tFitness newFitness = problem.fitness(solution, info, selectedElem, nonSelectedIdx);
                timer.stop();
                
                std::cout << "New fitness with factorization: " << newFitness << std::endl;
                std::cout << "Factorized evaluation time: " << timer.elapsed() * 1000 << " ms" << std::endl;
                
                // Apply the move to solution
                tSolution newSolution = solution;
                newSolution[selectedElem] = false;
                newSolution[mddInfo->nonSelected[nonSelectedIdx]] = true;
                
                // Calculate fitness from scratch for verification
                timer.reset();
                timer.start();
                tFitness verifyFitness = problem.fitness(newSolution);
                timer.stop();
                
                std::cout << "Verification fitness: " << verifyFitness << std::endl;
                std::cout << "Regular evaluation time: " << timer.elapsed() * 1000 << " ms" << std::endl;
                
                // Check if factorization is correct
                if (std::abs(newFitness - verifyFitness) < 1e-6) {
                    std::cout << "Factorization is correct!" << std::endl;
                } else {
                    std::cout << "ERROR: Factorization gives different result!" << std::endl;
                }
            }
        }
        
        // Cleanup
        delete info;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 
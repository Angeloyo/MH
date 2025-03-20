#include <problemmdd.h>
#include <random.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

// Constructor: loads problem data from file
ProblemMDD::ProblemMDD(const std::string& filename) : Problem() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    // Set instance name from filename (remove path and extension)
    size_t lastSlash = filename.find_last_of("/\\");
    size_t lastDot = filename.find_last_of(".");
    if (lastSlash == std::string::npos) {
        lastSlash = 0;
    } else {
        lastSlash++;
    }
    
    if (lastDot == std::string::npos || lastDot < lastSlash) {
        instanceName = filename.substr(lastSlash);
    } else {
        instanceName = filename.substr(lastSlash, lastDot - lastSlash);
    }

    // Read first line: n and m
    file >> n >> m;
    if (n <= 0 || m <= 0 || m >= n) {
        throw std::runtime_error("Error: Invalid n or m values in file");
    }

    // Initialize distance matrix with zeros
    distances.resize(n, std::vector<float>(n, 0.0f));

    // Read distance data (only upper triangular part)
    int i, j;
    float dist;
    while (file >> i >> j >> dist) {
        if (i < 0 || i >= n || j < 0 || j >= n) {
            throw std::runtime_error("Error: Invalid indices in distance matrix");
        }
        distances[i][j] = dist;
        distances[j][i] = dist; // Fill the symmetric part
    }

    file.close();
}

// Get distance between elements i and j
float ProblemMDD::getDistance(int i, int j) const {
    if (i < 0 || i >= n || j < 0 || j >= n) {
        throw std::out_of_range("Index out of range in getDistance");
    }
    return distances[i][j];
}

// Create a random valid solution with exactly m elements selected
tSolution ProblemMDD::createSolution() {
    tSolution solution(n, false); // Initialize all to false
    
    // Select m elements randomly
    std::vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }
    
    // Shuffle and select the first m elements
    Random::shuffle(indices.begin(), indices.end());
    for (int i = 0; i < m; i++) {
        solution[indices[i]] = true;
    }
    
    return solution;
}

// Evaluate a solution (calculate differential dispersion)
tFitness ProblemMDD::fitness(const tSolution& solution) {
    // Count the number of selected elements
    int count = 0;
    for (bool selected : solution) {
        if (selected) count++;
    }
    
    // Check if the solution is valid (has exactly m elements)
    if (count != m) {
        return std::numeric_limits<tFitness>::max(); // Return a very large value for invalid solutions
    }
    
    // Calculate the sum of distances for each selected element
    std::vector<float> sumDistances;
    for (int i = 0; i < n; i++) {
        if (solution[i]) {
            float sum = 0.0f;
            for (int j = 0; j < n; j++) {
                if (i != j && solution[j]) {
                    sum += distances[i][j];
                }
            }
            sumDistances.push_back(sum);
        }
    }
    
    // Find the maximum and minimum sum
    float maxSum = *std::max_element(sumDistances.begin(), sumDistances.end());
    float minSum = *std::min_element(sumDistances.begin(), sumDistances.end());
    
    // Return the differential dispersion (to be minimized)
    return maxSum - minSum;
}

// Generate factoring information for a solution
SolutionFactoringInfo* ProblemMDD::generateFactoringInfo(const tSolution& solution) {
    MDDSolutionInfo* info = new MDDSolutionInfo();
    
    // Store selected and non-selected elements
    for (int i = 0; i < n; i++) {
        if (solution[i]) {
            info->selected.push_back(i);
        } else {
            info->nonSelected.push_back(i);
        }
    }
    
    // Calculate sum of distances for each selected element
    info->sumDistances.resize(info->selected.size());
    for (size_t i = 0; i < info->selected.size(); i++) {
        int elemI = info->selected[i];
        float sum = 0.0f;
        for (size_t j = 0; j < info->selected.size(); j++) {
            if (i != j) {
                int elemJ = info->selected[j];
                sum += distances[elemI][elemJ];
            }
        }
        info->sumDistances[i] = sum;
    }
    
    return info;
}

// Factorized fitness calculation for local search
tFitness ProblemMDD::fitness(const tSolution& solution,
                           SolutionFactoringInfo* solution_info,
                           unsigned pos_change, tDomain new_value) {
    // We're implementing the Int(Sel,i,j) move: 
    // Swap a selected element (pos_change) with a non-selected one (new_value is index in nonSelected)
    
    MDDSolutionInfo* info = dynamic_cast<MDDSolutionInfo*>(solution_info);
    if (!info) {
        // Fallback to non-factorized if cast fails
        tSolution newSol(solution);
        newSol[pos_change] = new_value;
        return fitness(newSol);
    }
    
    // Find indices in selected and nonSelected arrays
    int selectedIdx = -1;
    for (size_t i = 0; i < info->selected.size(); i++) {
        if (info->selected[i] == pos_change) {
            selectedIdx = i;
            break;
        }
    }
    
    if (selectedIdx == -1 || new_value >= info->nonSelected.size()) {
        // Invalid move, return worst possible fitness
        return std::numeric_limits<tFitness>::max();
    }
    
    int nonSelectedElem = info->nonSelected[new_value];
    
    // Create a copy of the sum distances
    std::vector<float> newSumDistances = info->sumDistances;
    
    // Remove contribution of the element to be removed
    for (size_t i = 0; i < info->selected.size(); i++) {
        if (i != selectedIdx) {
            newSumDistances[i] -= distances[info->selected[i]][pos_change];
        }
    }
    
    // Add contribution of the new element
    for (size_t i = 0; i < info->selected.size(); i++) {
        if (i != selectedIdx) {
            newSumDistances[i] += distances[info->selected[i]][nonSelectedElem];
        }
    }
    
    // Calculate new sum for the swapped element
    float newSum = 0.0f;
    for (size_t i = 0; i < info->selected.size(); i++) {
        if (i != selectedIdx) {
            newSum += distances[nonSelectedElem][info->selected[i]];
        }
    }
    
    // Replace the old sum with the new one
    newSumDistances[selectedIdx] = newSum;
    
    // Find max and min in the new sums
    float maxSum = *std::max_element(newSumDistances.begin(), newSumDistances.end());
    float minSum = *std::min_element(newSumDistances.begin(), newSumDistances.end());
    
    // Return the differential dispersion
    return maxSum - minSum;
}

// Update factoring information after a move
void ProblemMDD::updateSolutionFactoringInfo(SolutionFactoringInfo* solution_info,
                                            const tSolution& solution,
                                            unsigned pos_change, tDomain new_value) {
    MDDSolutionInfo* info = dynamic_cast<MDDSolutionInfo*>(solution_info);
    if (!info) return;
    
    // Find indices in selected and nonSelected arrays
    int selectedIdx = -1;
    for (size_t i = 0; i < info->selected.size(); i++) {
        if (info->selected[i] == pos_change) {
            selectedIdx = i;
            break;
        }
    }
    
    if (selectedIdx == -1 || new_value >= info->nonSelected.size()) {
        return; // Invalid move
    }
    
    int nonSelectedElem = info->nonSelected[new_value];
    
    // Update sums for all other selected elements
    for (size_t i = 0; i < info->selected.size(); i++) {
        if (i != selectedIdx) {
            // Remove contribution of removed element
            info->sumDistances[i] -= distances[info->selected[i]][pos_change];
            // Add contribution of new element
            info->sumDistances[i] += distances[info->selected[i]][nonSelectedElem];
        }
    }
    
    // Calculate new sum for the swapped element
    float newSum = 0.0f;
    for (size_t i = 0; i < info->selected.size(); i++) {
        if (i != selectedIdx) {
            newSum += distances[nonSelectedElem][info->selected[i]];
        }
    }
    
    // Update the selected and nonSelected lists and the sum for the swapped element
    info->sumDistances[selectedIdx] = newSum;
    info->selected[selectedIdx] = nonSelectedElem;
    info->nonSelected[new_value] = pos_change;
} 
#include <greedymdd.h>
#include <problemmdd.h>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <limits>
#include <random.hpp>
#include <algorithm>

/**
 * Run the Greedy algorithm.
 * 
 * @param problem The MDD problem to solve
 * @param maxevals Maximum number of evaluations (ignored in greedy)
 * @return A ResultMH containing the solution found, its fitness, and the number of evaluations
 */
ResultMH GreedyMDD::optimize(Problem* problem, int maxevals) {
    // Comprobamos que es un problema MDD
    ProblemMDD* mddProblem = dynamic_cast<ProblemMDD*>(problem);
    assert(mddProblem != nullptr);
    
    // Inicializar temporizador
    Timer timer;
    timer.start();
    
    // Obtener n y m del problema
    int n = mddProblem->getN();
    int m = mddProblem->getM();
    
    // Inicializar la solución (todos a false)
    tSolution solution(n, false);
    
    // Contador de evaluaciones
    int evaluations = 0;
    
    // Vector para saber qué elementos ya han sido seleccionados y no seleccionados
    std::vector<int> selectedElements;
    std::vector<int> nonSelectedElements;
    for (int i = 0; i < n; i++) {
        nonSelectedElements.push_back(i);
    }
    
    // Seleccionamos el primer elemento aleatoriamente
    int randomIndex = Random::get<int>(0, n-1);
    int firstElement = nonSelectedElements[randomIndex];
    
    // Actualizar la solución y los vectores
    solution[firstElement] = true;
    selectedElements.push_back(firstElement);
    nonSelectedElements.erase(nonSelectedElements.begin() + randomIndex);
    
    std::cout << "Greedy: Seleccionado primer elemento " << firstElement << " aleatoriamente" << std::endl;
    
    // Matriz de distancias para cálculos parciales
    std::vector<std::vector<float>> distances(n, std::vector<float>(n, 0.0f));
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            distances[i][j] = mddProblem->getDistance(i, j);
            distances[j][i] = distances[i][j]; // Matriz simétrica
        }
    }
    
    // Seleccionamos los m-1 elementos restantes
    for (int iter = 1; iter < m; iter++) {
        double bestDisp = std::numeric_limits<double>::max();
        int bestElementIdx = -1;
        
        // Probar cada elemento no seleccionado
        for (size_t i = 0; i < nonSelectedElements.size(); i++) {
            int candidate = nonSelectedElements[i];
            
            // Calcular la dispersión que resultaría al añadir este elemento
            
            // Suma de distancias para cada elemento seleccionado (incluido el candidato)
            std::vector<float> sumDistances(selectedElements.size() + 1, 0.0f);
            
            // Calcular sumas para los ya seleccionados
            for (size_t j = 0; j < selectedElements.size(); j++) {
                int element = selectedElements[j];
                // Distancias a otros elementos seleccionados
                for (size_t k = 0; k < selectedElements.size(); k++) {
                    if (j != k) {
                        sumDistances[j] += distances[element][selectedElements[k]];
                    }
                }
                // Añadir distancia al candidato
                sumDistances[j] += distances[element][candidate];
            }
            
            // Calcular suma para el candidato
            for (size_t j = 0; j < selectedElements.size(); j++) {
                sumDistances[selectedElements.size()] += distances[candidate][selectedElements[j]];
            }
            
            // Calcular dispersión diferencial
            float maxSum = *std::max_element(sumDistances.begin(), sumDistances.end());
            float minSum = *std::min_element(sumDistances.begin(), sumDistances.end());
            float dispersion = maxSum - minSum;
            
            evaluations++;
            
            // Si mejora la dispersión, actualizamos el mejor elemento
            if (dispersion < bestDisp) {
                bestDisp = dispersion;
                bestElementIdx = i;
            }
        }
        
        // Añadimos el mejor elemento a la solución
        if (bestElementIdx != -1) {
            int bestElement = nonSelectedElements[bestElementIdx];
            solution[bestElement] = true;
            selectedElements.push_back(bestElement);
            nonSelectedElements.erase(nonSelectedElements.begin() + bestElementIdx);
            
            std::cout << "Greedy: Seleccionado elemento " << bestElement 
                      << " (dispersión: " << bestDisp << ")" << std::endl;
        } else {
            std::cerr << "Error: No se pudo encontrar un elemento para añadir." << std::endl;
            break;
        }
    }
    
    // Calculamos el fitness final
    double finalFitness = mddProblem->fitness(solution);
    evaluations++;
    
    // Detenemos el temporizador
    timer.stop();
    
    // Ordenar los elementos seleccionados para mostrarlos
    std::sort(selectedElements.begin(), selectedElements.end());
    
    // Mostrar resultados
    std::cout << "\nGreedy-MDD completado en " << std::fixed << std::setprecision(2)
              << timer.elapsed() << " segundos." << std::endl;
    std::cout << "Fitness final: " << finalFitness << std::endl;
    std::cout << "Elementos seleccionados: ";
    for (size_t i = 0; i < selectedElements.size(); i++) {
        std::cout << selectedElements[i];
        if (i < selectedElements.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
    
    // Devolver el resultado
    return ResultMH(solution, finalFitness, evaluations);
} 
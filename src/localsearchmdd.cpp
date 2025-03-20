#include <localsearchmdd.h>
#include <problemmdd.h>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random.hpp>

/**
 * Run the Local Search algorithm.
 * 
 * @param problem The MDD problem to solve
 * @param maxevals Maximum number of evaluations (stops at 100,000 or when no improvement is found)
 * @return A ResultMH containing the best solution found, its fitness, and the number of evaluations
 */
ResultMH LocalSearchMDD::optimize(Problem* problem, int maxevals) {
    // Comprobamos que es un problema MDD
    ProblemMDD* mddProblem = dynamic_cast<ProblemMDD*>(problem);
    assert(mddProblem != nullptr);
    
    // Máximo de evaluaciones (100,000 como indica el guión)
    const int MAX_EVALS = 100000;
    if (maxevals <= 0 || maxevals > MAX_EVALS) {
        maxevals = MAX_EVALS;
    }
    
    // Inicializar temporizador
    Timer timer;
    timer.start();
    
    // Obtener n y m del problema
    int n = mddProblem->getN();
    int m = mddProblem->getM();
    
    // Generamos una solución inicial aleatoria
    tSolution currentSolution = mddProblem->createSolution();
    
    // Evaluamos la solución inicial
    double currentFitness = mddProblem->fitness(currentSolution);
    int evaluations = 1;
    
    std::cout << "LocalSearch (" << getName() << "): Solución inicial con fitness " 
              << currentFitness << std::endl;
    
    // Generamos información de factorización para acelerar la búsqueda local
    SolutionFactoringInfo* factorInfo = mddProblem->generateFactoringInfo(currentSolution);
    
    // Vectores para almacenar elementos seleccionados y no seleccionados
    std::vector<int> selectedElements;
    std::vector<int> nonSelectedElements;
    
    for (int i = 0; i < n; i++) {
        if (currentSolution[i]) {
            selectedElements.push_back(i);
        } else {
            nonSelectedElements.push_back(i);
        }
    }
    
    // Flag para saber si se ha mejorado en la iteración actual
    bool improved = true;
    
    // Iteramos mientras haya mejora y no se supere el límite de evaluaciones
    while (improved && evaluations < maxevals) {
        improved = false;
        
        // Exploración del entorno (Int(Sel,i,j) - intercambiar seleccionado i por no seleccionado j)
        
        // Creamos vectores de índices para recorrer los elementos seleccionados y no seleccionados
        std::vector<int> selectedIndices(selectedElements.size());
        for (size_t i = 0; i < selectedIndices.size(); i++) {
            selectedIndices[i] = i;
        }
        
        std::vector<int> nonSelectedIndices(nonSelectedElements.size());
        for (size_t i = 0; i < nonSelectedIndices.size(); i++) {
            nonSelectedIndices[i] = i;
        }
        
        // Si es estrategia aleatoria, mezclamos los índices
        if (strategy == ExplorationStrategy::RANDOM) {
            Random::shuffle(selectedIndices.begin(), selectedIndices.end());
            Random::shuffle(nonSelectedIndices.begin(), nonSelectedIndices.end());
        }
        // Si es estrategia heurística, ordenamos los índices según la heurística
        else if (strategy == ExplorationStrategy::HEURISTIC) {
            // Calculamos la contribución de cada elemento a la dispersión
            std::vector<std::pair<int, float>> selectedContributions;
            std::vector<std::pair<int, float>> nonSelectedContributions;
            
            // Contribución de elementos seleccionados (cuánto contribuye cada elemento al fitness)
            MDDSolutionInfo* info = dynamic_cast<MDDSolutionInfo*>(factorInfo);
            if (info) {
                for (size_t i = 0; i < selectedIndices.size(); i++) {
                    selectedContributions.push_back({i, info->sumDistances[i]});
                }
                
                // Ordenamos de mayor a menor contribución (los que más contribuyen se exploran primero)
                // porque queremos quitar primero los que más contribuyen al fitness alto
                std::sort(selectedContributions.begin(), selectedContributions.end(), 
                        [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                            return a.second > b.second;
                        });
                
                // Actualizamos los índices ordenados
                for (size_t i = 0; i < selectedIndices.size(); i++) {
                    selectedIndices[i] = selectedContributions[i].first;
                }
                
                // Para los no seleccionados, calcularíamos su potencial contribución
                // Pero como no tenemos una manera directa de calcularlo, los dejamos en orden aleatorio
                Random::shuffle(nonSelectedIndices.begin(), nonSelectedIndices.end());
            }
        }
        
        // Exploramos el entorno (primer mejor)
        for (size_t i = 0; i < selectedIndices.size() && !improved && evaluations < maxevals; i++) {
            int selectedIdx = selectedIndices[i];
            int selectedElem = selectedElements[selectedIdx];
            
            for (size_t j = 0; j < nonSelectedIndices.size() && !improved && evaluations < maxevals; j++) {
                int nonSelectedIdx = nonSelectedIndices[j];
                int nonSelectedElem = nonSelectedElements[nonSelectedIdx];
                
                // Calcular fitness factorizado para el movimiento Int(Sel,i,j)
                double newFitness = mddProblem->fitness(currentSolution, factorInfo, selectedElem, nonSelectedIdx);
                evaluations++;
                
                // Si mejora, realizamos el movimiento
                if (newFitness < currentFitness) {
                    // Actualizar la solución y fitness
                    currentSolution[selectedElem] = false;
                    currentSolution[nonSelectedElem] = true;
                    currentFitness = newFitness;
                    
                    // Actualizar la información de factorización
                    mddProblem->updateSolutionFactoringInfo(factorInfo, currentSolution, selectedElem, nonSelectedIdx);
                    
                    // Actualizar los vectores de elementos
                    selectedElements[selectedIdx] = nonSelectedElem;
                    nonSelectedElements[nonSelectedIdx] = selectedElem;
                    
                    improved = true;
                    
                    std::cout << "LocalSearch (" << getName() << "): Mejora encontrada - Intercambio " 
                              << selectedElem << " por " << nonSelectedElem 
                              << " (nuevo fitness: " << currentFitness << ")" << std::endl;
                    
                    // En el esquema del primer mejor, rompemos el bucle al encontrar una mejora
                    break;
                }
            }
        }
    }
    
    // Liberamos la memoria de la información de factorización
    delete factorInfo;
    
    // Detenemos el temporizador
    timer.stop();
    
    // Mostrar resultados
    std::cout << "\nLocalSearch (" << getName() << ") completado en " << std::fixed << std::setprecision(2)
              << timer.elapsed() << " segundos." << std::endl;
    std::cout << "Fitness final: " << currentFitness << std::endl;
    std::cout << "Evaluaciones: " << evaluations << std::endl;
    
    // Devolver el resultado
    return ResultMH(currentSolution, currentFitness, evaluations);
} 
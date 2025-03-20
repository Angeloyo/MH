#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
// Real problem class
#include <problemmdd.h>

// All algorithms
#include <randomsearchmdd.h>
#include <greedymdd.h>
#include <localsearchmdd.h>

using namespace std;
int main(int argc, char *argv[]) {
  // Verificar argumentos
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <path_to_instance_file> <seed>" << std::endl;
    return 1;
  }

  // Obtener argumentos
  std::string instance_path = argv[1];
  long int seed = atoi(argv[2]);

  // Inicializar generador de números aleatorios
  Random::seed(seed);

  // Cargar la instancia del problema
  cout << "Loading problem instance from: " << instance_path << endl;
  ProblemMDD problem(instance_path);
  
  cout << "Instance: " << problem.getInstanceName() << endl;
  cout << "n = " << problem.getN() << ", m = " << problem.getM() << endl;

  // Crear los algoritmos
  RandomSearchMDD randomSearch;
  GreedyMDD greedy;
  LocalSearchMDD randLS(ExplorationStrategy::RANDOM);
  LocalSearchMDD heurLS(ExplorationStrategy::HEURISTIC);

  // Vector de algoritmos a ejecutar
  vector<pair<string, MH *>> algoritmos = {
    make_pair("RandomSearch", &randomSearch),
    make_pair("Greedy", &greedy),
    make_pair("randLS", &randLS),
    make_pair("heurLS", &heurLS)
  };

  // Ejecutar cada algoritmo
  for (auto& alg : algoritmos) {
    // Reiniciar la semilla para que cada algoritmo tenga la misma secuencia aleatoria
    Random::seed(seed);
    
    cout << "\n=== " << alg.first << " ===" << endl;
    
    // Ejecutar algoritmo (100,000 evaluaciones máximo como indica el guión)
    MH *mh = alg.second;
    Timer timer;
    timer.start();
    ResultMH result = mh->optimize(&problem, 100000);
    timer.stop();
    
    // Mostrar resultados
    cout << "Best fitness: " << result.fitness << endl;
    cout << "Evaluations: " << result.evaluations << endl;
    cout << "Time: " << timer.elapsed() << " seconds" << endl;
    
    // Mostrar solución
    cout << "Best solution: [";
    bool first = true;
    for (size_t i = 0; i < result.solution.size(); i++) {
      if (result.solution[i]) {
        if (!first) cout << ", ";
        cout << i;
        first = false;
      }
    }
    cout << "]" << endl;
  }

  return 0;
}

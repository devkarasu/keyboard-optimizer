#pragma once
#include <string>
#include <vector>

namespace ga {
  struct ConstGAParam {
    static const int POPULATION_SIZE;
    static const int GENERATION_MAX;
    static const int CROSS_RATE;
    static const int MUTATION_RATE;
    static const int TOURNAMENT_SIZE;
  };

  using Population = std::vector<Individual>;

  class Individual {
    std::string _layout;
    double _fitness;

  public:
    const std::string& layout() const;
    std::string layout(const std::string& _layout);

    double fitness() const;

    Individual();
    Individual(const Individual& rhs);
    double calcFitness();
  };

  class Generation {
    Population population;

  public:
    Generation();
    Generation& nextGeneration();
  };

  Individual& selection(Population& x);
  void crossover(Individual& a, Individual& b);
  void mutation(Population& x);
}

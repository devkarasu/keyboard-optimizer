#pragma once
#include <string>
#include <vector>

namespace ga {
  struct ConstParam {
    static const int POPULATION_SIZE;
    static const double CROSS_RATE;
    static const double MUTATION_RATE;
    static const long long RANDOM_SEED;
  };

  class Individual {
    std::string _layout;
    double _fitness;

  public:
    std::string layout() const;
    void layout(const std::string& _layout);

    double calcFitness();
    double fitness() const;

    Individual();
    Individual(const Individual& rhs);
  };

  using Population = std::vector<Individual>;

  class Generation {
    Population population;

  public:
    Generation();
    void nextGeneration();
    void calcFitness();
    Individual getElite() const;
    void printFitnesses() const;
  };

  int loadFrequency();
}


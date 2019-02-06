#pragma once
#include <string>
#include <vector>

namespace ga {
  struct ConstParam {
    static const int POPULATION_SIZE;
    static const int GENERATION_MAX;
    static const double CROSS_RATE;
    static const double MUTATION_RATE;
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
    Individual getElite() const;
  };
}

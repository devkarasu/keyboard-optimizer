#include "genetic_algorithm.h"
#include <iostream>
#include <numeric>
#include <random>
#include <algorithm>
#include <regex>
using namespace ga;

const int ConstParam::POPULATION_SIZE = 100;
const int ConstParam::GENERATION_MAX = 1000;
const double ConstParam::CROSS_RATE = 95;
const double ConstParam::MUTATION_RATE = 0.5;

const int LEN = 25;

std::random_device rnd;
std::mt19937_64 mt(rnd());
std::uniform_real_distribution<> percentage(0.0, 100.0);

std::string Individual::layout() const{
  return _layout;
}

void Individual::layout(const std::string& _layout) {
  this->_layout = _layout;
}

double Individual::fitness() const {
  return _fitness;
}

Individual::Individual() : _fitness(0.0) {
  _layout = "qwertyuiopasdfghjklzxcvbnm";
  std::uniform_int_distribution<> randomIndex(0, LEN);
  for (int i = 0; i < 500; ++i) {
    int x1 = randomIndex(mt);
    int x2 = randomIndex(mt);

    auto tmp = _layout[x1];
    _layout[x1] = _layout[x2];
    _layout[x2] = tmp;
  }
  std::cout << layout() << std::endl;
}

Individual::Individual(const Individual& rhs) : _layout(rhs._layout), _fitness(rhs._fitness) {};

Generation::Generation() {
  population.resize(ConstParam::POPULATION_SIZE + 1);
}

Individual selection(const Population& x) {
  double roulette_max = std::accumulate(std::begin(x), std::end(x), 0.0,
    [](double lhs, const Population::value_type& rhs) { return lhs + 200 - rhs.fitness(); }
  );

  std::uniform_real_distribution<> roulette(0, roulette_max);

  double dart = roulette(mt);
  double sum = 0.0;
  int i;
  for (i = 0; dart > sum && i < x.size(); i++) {
    sum = 200 - x[i].fitness() + sum;
  }

  return x[i - 1];
}

void sortByMom(const std::string& mom, std::string& daughter) {
  std::string result = "";

  for (int i = 0; i < mom.length(); ++i) {
    if (daughter.find(mom[i]) != std::string::npos) {
      result += mom[i];
    }
  }
  daughter = result;
}

void crossover(Individual& a, Individual& b) {
  std::uniform_int_distribution<> randomIndex(1, LEN);
  int x = randomIndex(mt);

  std::string moms[] = { b.layout(), a.layout() };
  std::string childs[] = { a.layout(), b.layout() };

  for (int i = 0; i < 2; ++i) {
    childs[i].erase(std::begin(childs[i]), std::begin(childs[i]) + x);
    sortByMom(moms[i], childs[i]);
  }

  a.layout(moms[1].substr(0, x) + childs[0]);
  b.layout(moms[0].substr(0, x) + childs[1]);
}

void mutation(Population& p) {
  std::uniform_int_distribution<> randomIndex(0, LEN);
  for (int i = 0; i < p.size(); i++) {
    if (percentage(mt) > ConstParam::MUTATION_RATE) {
      continue;
    }

    int x1 = randomIndex(mt);
    int x2 = randomIndex(mt);
    std::string l = p[i].layout();

    std::string::value_type tmp = l[x1];
    l[x1] = l[x2];
    l[x2] = tmp;

    p[i].layout(l);
  }
}

void Generation::nextGeneration() {
  Population next_population;
  next_population.reserve(ConstParam::POPULATION_SIZE + 1);

  for (auto& x : population) {
    x.calcFitness();
  }

  for (int i = 0; i < ConstParam::POPULATION_SIZE / 2; i++) {
    auto a = selection(this->population);
    auto b = selection(this->population);

    crossover(a, b);

    next_population.push_back(a);
    next_population.push_back(b);
  }

  mutation(next_population);

  auto min = [](auto lhs, auto rhs) {
    return lhs.fitness() > rhs.fitness();
  };
  auto x = std::min_element(std::begin(population), std::end(population), min);
  next_population.push_back(*x);

  this->population = next_population;
}

Individual Generation::getElite() const {
  auto min = [](auto lhs, auto rhs) {
    return lhs.fitness() > rhs.fitness();
  };
  auto x = std::min_element(std::begin(population), std::end(population), min);
  return *x;
}

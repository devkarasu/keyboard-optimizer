#include "genetic_algorithm.h"
#include <iostream>
#include <numeric>
#include <random>
#include <algorithm>
#include <regex>
using namespace ga;

const int ConstParam::POPULATION_SIZE = 2000;
const double ConstParam::CROSS_RATE = 95;
const double ConstParam::MUTATION_RATE = 0.5;

std::random_device rnd;
const long long ConstParam::RANDOM_SEED = rnd();

const int LEN = 25;

std::mt19937_64 mt(ConstParam::RANDOM_SEED);
std::uniform_real_distribution<> rdist(0.0, 1.0);

const auto compareIndividual = [](auto lhs, auto rhs) {
  return lhs.fitness() < rhs.fitness();
};

double percentage() {
  return rdist(mt) * 100.0;
}

double roulette(double max) {
  return rdist(mt) * max;
}

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
  calcFitness();
}

Individual selection(const Population& x) {
  static const double fitness_offset = 200;
  double roulette_max = std::accumulate(std::begin(x), std::end(x), 0.0,
    [](double lhs, const Population::value_type& rhs) { return lhs + rhs.fitness(); }
  );

  roulette_max = fitness_offset * x.size() - roulette_max;

  double dart = roulette(roulette_max);
  double sum = 0.0;
  int i;
  for (i = 0; dart > sum && i < x.size(); i++) {
    sum = fitness_offset - x[i].fitness() + sum;
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
    if (percentage() > ConstParam::MUTATION_RATE) {
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

  for (int i = 0; i < ConstParam::POPULATION_SIZE / 2; i++) {
    auto a = selection(this->population);
    auto b = selection(this->population);

    crossover(a, b);

    next_population.push_back(a);
    next_population.push_back(b);
  }

  mutation(next_population);

  next_population.push_back(getElite());

  this->population = next_population;
  calcFitness();
}

Individual Generation::getElite() const {
  auto x = std::min_element(std::begin(population), std::end(population), compareIndividual);
  return *x;
}

void Generation::calcFitness() {
  for (auto& x : population) {
    x.calcFitness();
  }
}

void Generation::printFitnesses() const {
  std::cout << "----------fitness-----------------" << std::endl;
  for (auto i : population)
    std::cout << i.fitness() << std::endl;
  std::cout << "----------------------------------" << std::endl;
}

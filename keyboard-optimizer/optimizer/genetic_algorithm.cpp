#include "genetic_algorithm.h"
#include <numeric>
#include <random>
using namespace ga;

const int ConstParam::POPULATION_SIZE = 100;
const int ConstParam::GENERATION_MAX = 3000;
const double ConstParam::CROSS_RATE = 95;
const double ConstParam::MUTATION_RATE = 0.5;

std::random_device rnd;
std::mt19937_64 mt(rnd());
std::uniform_real_distribution<> percentage(0.0, 100.0);

const std::string& Individual::layout() const{
  return this->_layout;
}

void Individual::layout(const std::string& _layout) {
  this->_layout = _layout;
}

double Individual::fitness() const {
  if (_fitness > 1.0e-5) {
    return _fitness;
  }

  // TODO: Write layout evaluation here.
  std::uniform_real<> f(1.0, 100.0);
  return f(mt);
}

Individual::Individual() : _fitness(0.0) {
  _layout = "qwertyuiopasdfghjklzxcvbnm";
}

Individual::Individual(const Individual& rhs) : _layout(rhs._layout), _fitness(rhs._fitness) {};

Generation::Generation() {
  population.resize(ConstParam::POPULATION_SIZE);
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

// 順序交叉の並べ替えを行う
std::string sortByMom(const std::string& mom, std::string& daughter) {
  std::string result = "";

  for (int i = 0; i < mom.length(); ++i) {
    if (daughter.find(mom[i])) {
      result += mom[i];
    }
  }

  daughter = result;
}

void crossover(Individual& a, Individual& b) {
  std::uniform_int_distribution<> randomIndex(0, a.layout().length());
  int x = randomIndex(mt);

  std::string moms[] = { b.layout(), a.layout() };
  std::string childs[] = { a.layout(), b.layout() };

  for (int i = 0; i < 2; ++i) {
    // 並び替える部分の抽出
    childs[i].erase(std::begin(childs[i]), std::begin(childs[i]) + x);
    // 並び替え
    sortByMom(moms[i], childs[i]);
    // 親から受け継ぐ部分の抽出
    moms[i].erase(std::begin(moms[i]) + x, std::end(moms[i]));
  }

  a.layout(moms[1] + childs[0]);
  b.layout(moms[0] + childs[1]);
}

// 突然変異 転座
void mutation(Population& p) {
  for (int i = 0; i < p.size(); i++) {
    if (percentage(mt) > ConstParam::MUTATION_RATE) {
      continue;
    }

    std::uniform_int_distribution<> randomIndex(0, p.size());

    int x1 = randomIndex(mt);
    int x2 = randomIndex(mt);

    std::string l = p[i].layout();

    auto tmp = l[x1];
    l[x2] = l[x1];
    l[x1] = tmp;

    p[i].layout(l);
  }
}

// 次の世代へ
Generation& Generation::nextGeneration() {
  Population next_population;
  next_population.reserve(ConstParam::POPULATION_SIZE);

  for (int i = 0; i < ConstParam::POPULATION_SIZE / 2; i++) {
    auto a = selection(this->population);
    auto b = selection(this->population);

    crossover(a, b);

    next_population.push_back(a);
    next_population.push_back(b);
  }

  mutation(next_population);

  // エリートを残す
  next_population.push_back(*std::min_element(std::begin(this->population), std::end(this->population), [](const Population::value_type& x) {
    return x.fitness();
  }));

  this->population = next_population;
  return *this;
}

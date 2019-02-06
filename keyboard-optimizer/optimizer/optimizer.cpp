#include <iostream>
#include "genetic_algorithm.h"

void printKeys(const ga::Individual& x) {
  std::cout << "fitness:" << x.fitness() << std::endl;
  std::cout << x.layout() << std::endl;
  return;
}

int main(){
  std::cout << "Hello,World!" << std::endl;

  ga::Generation g;

  for (int i = 0; i < ga::ConstParam::GENERATION_MAX; i++) {
    std::cout << i << std::endl;
    g.nextGeneration();

    if (i % 500 == 0)
      std::cout << "elite:" << g.getElite().fitness() << std::endl;
  }

  printKeys(g.getElite());
  return 0;
}

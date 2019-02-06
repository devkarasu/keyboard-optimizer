#include <iostream>
#include <sstream>
#include "genetic_algorithm.h"

void printKeys(const ga::Individual& x) {
  std::cout << "fitness:" << x.fitness() << std::endl;

  std::istringstream s(x.layout());

  std::cout << "| ";
  for (int i = 0; i < 10; i++) 
    std::cout << (char)s.get() << " | ";
  std::cout << "\n| ";
  for (int i = 0; i < 9; i++) 
    std::cout << (char)s.get() << " | ";
  std::cout << "\n| ";
  for(int i = 0; i < 7; i++)
    std::cout << (char)s.get() << " | ";
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

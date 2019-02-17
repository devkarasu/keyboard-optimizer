#include <iostream>
#include <sstream>
#include <fstream>
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

  // Load 2-Gram data
  ga::loadFrequency();

  ga::Generation g;
  g.printFitnesses();

  std::ofstream result("result.csv");

  int count = 0;
  double pre_elite = g.getElite().fitness();
  for (int i = 0;; i++) {
    std::cout << i << std::endl;
    g.nextGeneration();

    if (i % 200 == 0) {
      auto elite = g.getElite();
      result << i << "," << elite.fitness() << "," << elite.layout() << std::endl;
      std::cout << "elite:" << elite.fitness() << std::endl;
    }

    if (g.getElite().fitness() == pre_elite)
      ++count;
    else {
      pre_elite = g.getElite().fitness();
      count = 0;
      std::cout << "Change elite" << std::endl;
    }
  }

  std::cout << "seed:" << ga::ConstParam::RANDOM_SEED << std::endl;
  printKeys(g.getElite());
  return 0;
}

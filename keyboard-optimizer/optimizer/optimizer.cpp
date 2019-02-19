#include <iostream>
#include <sstream>
#include <fstream>
#include "genetic_algorithm.h"

void printKeys(const ga::Individual& x, std::ostream& out) {
  out << "fitness:" << x.fitness() << std::endl;

  std::istringstream s(x.layout());

  out << "| ";
  for (int i = 0; i < 10; i++) 
    out << (char)s.get() << " | ";
  out << "\n | ";
  for (int i = 0; i < 9; i++) 
    out << (char)s.get() << " | ";
  out << "\n  | ";
  for(int i = 0; i < 7; i++)
    out << (char)s.get() << " | ";
  return;
}


int main(){
  std::cout << "Hello,World!" << std::endl;

  // Load 2-Gram data
  ga::loadFrequency();

  ga::Generation g;
  g.printFitnesses();

  std::ofstream result_text("final_layout.txt");
  result_text << "seed:" << ga::ConstParam::RANDOM_SEED << std::endl;

  std::ofstream result("result.csv");

  int count = 0;
  auto elite = g.getElite();
  double pre_elite = elite.fitness();
  result << -1 << "," << elite.fitness() << "," << elite.layout() << std::endl;
  for (unsigned long long i = 0;; i++) {
    g.nextGeneration();

    if (i % 500 == 0) {
      elite = g.getElite();
      std::cout << i << " elite:" << elite.fitness() << std::endl;
    }

    elite = g.getElite();
    if (elite.fitness() == pre_elite)
      ++count;
    else {
      pre_elite = elite.fitness();
      count = 0;
      std::cout << "Change elite" << std::endl;
      result << i << "," << elite.fitness() << "," << elite.layout() << std::endl;
    }

    if (count > 100000) {
      printKeys(elite, result_text);
      break;
    }
  }

  return 0;
}

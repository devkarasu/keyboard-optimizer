#include <iostream>
#include "genetic_algorithm.h"

int main(){
  std::cout << "Hello,World!" << std::endl;

  ga::Generation g;

  for (int i = 0; i < ga::ConstParam::GENERATION_MAX; i++) {
    std::cout << i << std::endl;
    g.nextGeneration();
  }
  return 0;
}

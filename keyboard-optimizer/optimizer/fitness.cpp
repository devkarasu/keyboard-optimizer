#include "genetic_algorithm.h"
#include <fstream>
using namespace ga;

double Individual::calcFitness() {
  // TODO: Write layout evaluation here.
  static double a = 10.0;
  a += 1.0;
  return _fitness;
}

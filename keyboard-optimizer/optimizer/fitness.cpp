#include "genetic_algorithm.h"
#include <fstream>
#include <iostream>
using namespace ga;

/* Layout Indexes
** |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |
** | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 |
** | 19 | 20 | 21 | 22 | 23 | 24 | 25 |
*/

int scores[] = {
    1, 3, 4, 3, 1, 1, 3, 4, 3, 1,
    5, 6, 6, 6, 2, 2, 6, 6, 6,
    1, 2, 2, 3, 1, 1, 3
};

double Individual::calcFitness() {
  std::ifstream samples("sample.txt");

  if (!samples) {
    std::cout << "i can't read sample texts\n";
    _fitness = 777;
    return _fitness;
  }

  char chr;
  int sum_score = 0;
  while (samples.get(chr)) {
    // Scores of key position
    auto i = _layout.find(chr);
    if (i != std::string::npos)
      sum_score += 7 - scores[i];
    else
      std::cout << "???\n";
  }

  _fitness = 1.0 * sum_score;
  return _fitness;
}

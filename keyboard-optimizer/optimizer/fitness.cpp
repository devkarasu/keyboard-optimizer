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
    6, 4, 3, 4, 6, 6, 4, 3, 4, 6,
    2, 1, 1, 1, 5, 5, 1, 1, 1,
    6, 5, 5, 4, 6, 6, 4
};

double Individual::calcFitness() {
  std::ifstream samples("sample.txt");

  if (!samples) {
    std::cout << "i can't read sample texts\n";
    _fitness = 777;
    return _fitness;
  }

  char chr;
  int score_pos = 0;
  while (samples.get(chr)) {
    // Scores of key position
    auto i = _layout.find(chr);
    if (i != std::string::npos)
      score_pos += scores[i];
    else
      std::cout << "???\n";
  }

  _fitness = 100 / 7000.0 * score_pos;
  return _fitness;
}

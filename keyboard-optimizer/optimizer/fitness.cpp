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

enum HAND {
  LEFT, RIGHT, NA
};

HAND judgeHand(size_t index) {
  if (index <= 4 || (10 <= index && index <= 14) || (19 <= index && index <= 23))
    return LEFT;
  else
    return RIGHT;
}

double Individual::calcFitness() {
  std::ifstream samples("sample.txt");

  if (!samples) {
    std::cout << "i can't read sample texts\n";
    _fitness = 777;
    return _fitness;
  }

  char chr;

  HAND pre_used_hand = NA;
  int right_count = 0;

  // Scores
  int score_pos = 0;
  int score_left_right = 0;
  double score_ratio;

  // Main Loop
  while (samples.get(chr)) {
    // tmp variables
    auto i = _layout.find(chr);
    auto hand = judgeHand(i);

    // Scores of key position
    if (i != std::string::npos)
      score_pos += scores[i];
    else
      std::cout << "???\n";

    // Usage rate of both hands
    if (pre_used_hand != hand) {
      ++score_left_right;
      pre_used_hand = hand;
    }

    // Ratio of left and right usage rate
    if (hand == RIGHT)
      ++right_count;
  }

  score_left_right = 1500 - score_left_right;

  score_ratio = std::abs(right_count / 1500.0 - 0.5) * 200;

  _fitness = 0.5 * (100 / 7000.0 * score_pos) + 0.4 * (100 / 1500.0 * score_left_right) + 0.1 * (score_ratio);
  return _fitness;
}

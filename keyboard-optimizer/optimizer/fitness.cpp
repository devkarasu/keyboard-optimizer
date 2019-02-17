#include "genetic_algorithm.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
using namespace ga;

/* Layout Indexes
** |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |
** | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 |
** | 19 | 20 | 21 | 22 | 23 | 24 | 25 |
*/

using FrequencyDataMap = std::unordered_map<std::pair<char, char>, double>;
FrequencyDataMap char_pair_frequency;

int loadFrequency() {
  std::ifstream data("frequency_data.csv");
  if (!data) {
    std::cout << "missing data" << std::endl;
    return 0;
  }

  char c1, c2;
  double f;
  int count = 0;

  while (!data.eof()) {
    data >> c1 >> c2 >> f;
    char_pair_frequency[std::make_pair(c1, c2)] = f;
    count++;
  }

  return count;
}

enum HAND { LEFT, RIGHT, NA };
HAND hand[26]{
  LEFT, LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT, RIGHT, RIGHT, RIGHT,
  LEFT, LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT, RIGHT, RIGHT,
  LEFT, LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT
};

enum FINGER { THUMB, INDEX, MIDDLE, RING, PINKY, NA };
FINGER checkFinger(size_t index) {
  static FINGER finger[] = {
    PINKY, RING, MIDDLE, INDEX, INDEX, INDEX, INDEX, MIDDLE, RING, PINKY
  };

  return finger[index % 10];
}
std::unordered_map<FINGER, double> finger_weight = {
  {THUMB, 3},
  {INDEX, 2},
  {MIDDLE, 1},
  {RING, 4.5},
  {PINKY, 4.5}
};

int row[26] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 
  2, 2, 2, 2, 2, 2, 2
};
double row_weight[3] = {
  2.0, 1.0, 3.0
};

HAND judgeHand(size_t index) {
  if (index <= 4 || (10 <= index && index <= 14) || (19 <= index && index <= 23))
    return LEFT;
  else
    return RIGHT;
}

int checkColumn(size_t index) {
  if (0 <= index && index <= 9) {
    return 3;
  }
  else if (10 <= index && index <= 18) {
    return 2;
  }
  else {
    return 1;
  }
}

double Individual::calcFitness() {
  std::ifstream samples("sample.txt");

  if (!samples) {
    std::cout << "i can't read sample texts\n";
    _fitness = 777;
    return _fitness;
  }

  // Main Loop
  while (samples.get(chr)) {
    // tmp variables
    auto i = _layout.find(chr);
    auto hand = judgeHand(i);
  }

 return _fitness;
}


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
const HAND hand[26]{
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

const int row[26] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 
  2, 2, 2, 2, 2, 2, 2
};
const double row_weight[3] = {
  2.0, 1.0, 3.0
};

double Individual::calcFitness() {
  _fitness = 0;

  for (auto two_gram : char_pair_frequency) {
    double freq = two_gram.second; // Frequency
    auto i1 = _layout.find(two_gram.first.first); // character1 of pair
    auto i2 = _layout.find(two_gram.first.second); // character2 of pair

    auto f1 = checkFinger(i1);
    auto f2 = checkFinger(i2);

    double h = std::abs(hand[i1] - hand[i2]);
    double r = std::abs(row[i1] - hand[i2]);
    double f = h == 0 ? 0 : std::abs(f1 - f2);
    double R = row_weight[row[i1]] + row_weight[row[i2]];
    double F = finger_weight[f1] + finger_weight[f2];

    _fitness += (185.8 - 40.0*h + 18.3*r - 11.0*f + 0.514*R + 1.07*F) * freq;
  }

  return _fitness;
}


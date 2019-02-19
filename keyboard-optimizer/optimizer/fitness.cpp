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

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator() (const std::pair<T1, T2> &pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

using FrequencyDataMap = std::unordered_map<std::pair<char, char>, double, pair_hash>;
FrequencyDataMap char_pair_frequency;

int ga::loadFrequency() {
  std::ifstream data("frequency_data.txt");
  if (!data) {
    std::cout << "missing data" << std::endl;
    return 0;
  }

  char c1, c2;
  double f;
  int count = 0;

  std::cout << "---------------data------------------\n";
  while (!data.eof()) {
    data >> c1 >> c2 >> f;
    if (f < 1.0e-8)
      continue;

    std::cout << c1 << " " << c2 << " " << f << std::endl;
    char_pair_frequency[std::make_pair(c1, c2)] = f;
    count++;
  }
  std::cout << "-------------------------------------\n";

  return count;
}

enum HAND { LEFT, RIGHT };
const HAND hand[26]{
  LEFT, LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT, RIGHT, RIGHT, RIGHT,
  LEFT, LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT, RIGHT, RIGHT,
  LEFT, LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT
};

enum FINGER { INDEX1, INDEX2, MIDDLE, RING, PINKY };
FINGER checkFinger(size_t index) {
  static FINGER finger[] = {
    PINKY, RING, MIDDLE, INDEX2, INDEX1, INDEX1, INDEX2, MIDDLE, RING, PINKY
  };

  if (index < 10)
	  return finger[index];
  else if (index < 19)
	  return finger[index - 10];
  else
	  return finger[index - 19];
}

double finger_weight[] = { 3, 2, 1, 4.5, 4.5 };

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
  size_t memo[26];
  int i = 0;
  for (auto c : _layout) {
    memo[c - 'a'] = i++;
  }

  for (auto two_gram : char_pair_frequency) {
    double freq = two_gram.second; // Frequency
    size_t i1, i2;

    // index of key1
    i1 = memo[two_gram.first.first - 'a'];

    // index of key2
    i2 = memo[two_gram.first.second - 'a'];

    auto f1 = checkFinger(i1);
    auto f2 = checkFinger(i2);

    double h = std::abs(hand[i1] - hand[i2]); // Hand Transition
    double r = h == 0 ? std::abs(row[i1] - row[i2]) : 0; // Row Transition
    double f = h == 0 ? std::abs(f1 - f2) : 0; // Finger Transition
    double R = row_weight[row[i1]] + row_weight[row[i2]]; // Row Weight
    double F = finger_weight[f1] + finger_weight[f2]; // Finger Weight
    _fitness += (185.8 - 40.0*h + 18.3*r - 11.0*f + 0.514*R + 1.07*F) * freq;
  }

  return _fitness;
}


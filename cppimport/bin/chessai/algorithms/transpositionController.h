#ifndef TRANSPOSITIONCONTROLLER_H
#define TRANSPOSITIONCONTROLLER_H

#include <map>
class TranspositionController {
 public:
  TranspositionController(const Board& board);
 private:
  std::vector<std::vector<std::map<std::string, int> >>;
};

#endif //TRANSPOSITIONCONTROLLER_H

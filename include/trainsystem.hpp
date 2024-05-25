#ifndef TRAINSYSTEM_HPP
#define TRAINSYSTEM_HPP

#include "train.hpp"

struct trainsystem{
  TrainManager TM;
  void Init(char* file_name) {TM.Init(file_name);}
  void add_train(std::stringstream& in);
  void delete_train(std::stringstream& in);
  void release_train(std::stringstream& in);
  TrainInfo get_TrainInfo(const StationT& str, const int& id);
  bool check(const int&, const int&, const int&, const int&);
  void query_train(std::stringstream& in);
  void query_ticket(std::stringstream& in);
  void query_transfer(std::stringstream& in);
  void remove() {
    TM.remove();
  }
};

#endif
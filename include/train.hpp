#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "bplustree.hpp"

struct Train{
  sjtu::string<20> trainID;
  int stationNum, seatNum;
  sjtu::string<30> stations[100];
  int prices[100];
  int startTime;
  short travelTimes[100];
  short stopoverTimes[100];
  char type;
  int saleDate;

};

struct TrainManager {
  
};


#endif
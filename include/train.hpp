#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "bplustree.hpp"
#include "config.h"
#include <sstream>
#include <string>

// query_ticket
// one stop form stoptime -> starttime id -> trainid
// travel form: starttime -> stoptime id -> release_id
extern TrainM basic;

struct TrainInfo{
  TrainInfo() = default;
  short stid, edid; // station id
  short saleDate0, saleDate1;// 
  clck st;
  int price; 
  int stoptime, starttime; 
  int id; // trainid 
  // start date
  TrainInfo(short saledate_0, short saledate_1, clck st_, 
  short stid_ = 0, short edid_ = 0, 
  int price_ = 0, 
  int stoptime_ = 0, int starttime_ = 0, 
  int id_ = 0) :
  price(price_), saleDate0(saledate_0),saleDate1(saledate_1), 
  stoptime(stoptime_), st(st_), starttime(starttime_), id(id_),
  stid(stid_), edid(edid_) {};
  bool operator < (const TrainInfo& rhs) const {
    return id < rhs.id;
    // 注意已经按id排序 加一个限定的find 额外getvalue多一个东西 只找出id的值
  }
  bool operator == (const TrainInfo& rhs) const {
    return id == rhs.id;
  }
};

struct Train{
  Train() = default;
  TrainT trainID;
  int stationNum, seatNum;
  StationT stations[20];
  int prices[20];
  short startTime;
  short travelTimes[20];
  short stopoverTimes[20];
  short saleDate[2];
  char type;
  void Init(std::stringstream& in);
  template<typename T>
  void Init_string(T* idx, std::string& input) {
    size_t cur = 0, nxt;
    while((nxt = input.find('|', cur)) != std::string::npos) {
      idx->Init(input.substr(cur, nxt - cur));
      ++idx;
      cur = nxt+1;
    }
    idx->Init(input.substr(cur));
  }
  void Init_time(short* idx, std::string& input);
  void Init_int(int* idx, std::string& input);
  void Init_short(short* idx, std::string& input);
};

struct Reinfo{
  int seat[20]; 
  int num, stop;
  Reinfo(int num_ = 0, int stop_ = 0) {
    num = num_, stop = stop_;
    for(int i = 0; i < stop; ++i)
      seat[i] = num;
  }
};
struct TrainManager {
  // id : begin ~ end no more than 90 copies from saledate; -id -d -> -Tid;
  // query_train basic_info; ticket: seat[100] * 90 trains
  void exit();
  //Timer train_timer;
  Bptree::BPlusTree<pair<unsigned int,int>, int, unsigned int> TrainID;
  Bptree::BPlusTree<pair<int,int>, int, int> release;
  // from stations to the id price time
  Bptree::BPlusTree<pair<unsigned int, int>, TrainInfo, unsigned int, 1145> seat;
  // steal? find second -intmax min
  FileManager<Train> TrainFile;
  FileManager<Reinfo> ReFile;
  void Init(std::string file_name) {
    TrainFile.Init(file_name + "_ID");
    ReFile.Init(file_name + "_Re");
    seat.Init(file_name + "_seat");
    TrainID.Init(file_name + "_id");
    release.Init(file_name + "_release");
  }
  void remove() {
    TrainFile.remove();
    ReFile.remove();
    TrainID.remove();
    release.remove();
  }
  TrainInfo get_one_TrainInfo(const StationT& name, const int& id);
  void add_train(std::stringstream& in);
  void print(const TrainInfo&, clck, const short&, const StationT&, const StationT&, const int&);
  int find_train(const unsigned int&);
  int find_release(const int& id);
  int get_seat(int index, int st, int ed);
  int update_seat(int index, int st, int ed, int num);
  void delete_train(std::stringstream& in);
  void release_train(std::stringstream& in);
  void query_train(std::stringstream& in);
  void query_ticket(std::stringstream& in);
  void query_transfer(std::stringstream& in);
};
// query_ticket  buy_ticket must be very quick!

#endif
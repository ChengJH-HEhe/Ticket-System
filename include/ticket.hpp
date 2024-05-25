#ifndef TICKET_HPP
#define TICKET_HPP

#include "bplustree.hpp"
#include "config.h"


struct Loginfo{
  TrainT name;
  int trainid, saledate;
  StationT sts, eds;
  clck s, stt, edt; // 起始站s的时间：存在name->trainid中, 该站的开始、结束
  short stid, edid; // {trainid, s, id, price} 
  int price;// 花费 
  int num;// 购买数量
  bool type; // 是否补票
  Loginfo() = default;
  Loginfo(int saledate_, TrainT name_, int trainid_, StationT sts_, StationT eds_, short stid_, short edid_, clck s_, clck stt_, clck edt_, int price_, int num_) : 
    saledate(saledate_), name(name_), trainid(trainid_), sts(sts_), eds(eds_), stid(stid_), edid(edid_), s(s_), stt(stt_), edt(edt_), price(price_), num(num_) {};
};

struct ticketsystem {
  // <订单号, > - sta，id->订单号
  FileManager<Loginfo> Flog;
  // 
  Bptree::BPlusTree<pair<int, pair<int,int>>, pair<int,int>, int> ulog;
  // 车次 订单号 -> 订单号
  Bptree::BPlusTree<pair<int,int>, int, int> pnd;
  void Init(char* file_name) {
    Flog.Init(file_name);
    ulog.Init("ulog");
    pnd.Init("pnd");
  }
  void remove() {
    Flog.remove();
    ulog.remove();
    pnd.remove();
  }
  // refund ticket: ulog remove, if(pnd.find) pnd remove ; 
  // get all trainid loginfo, check again if they are valid ?
  //
  void buy_ticket(const int& uid, const Loginfo& info, const bool& type);
  void query_order(std::stringstream& in);
  sjtu::vector<int> refund_ticket(const int& uid, const int& id, int& tid);
};

#endif
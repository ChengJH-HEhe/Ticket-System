#ifndef TICKET_HPP
#define TICKET_HPP

#include "bplustree.hpp"
#include "config.h"


struct Loginfo{
  TrainT name;
  int seatid, saledate; // 
  StationT sts, eds;
  clck s, stt, edt; // 起始站s的时间：存在name->trainid中, 该站的开始、结束
  short stid, edid; // {trainid, s, id, price} 
  int price;// 花费 
  int num;// 购买数量
  bool type; // 是否补票
  Loginfo() = default;
  Loginfo(int saledate_, TrainT name_, int seatid_, StationT sts_, StationT eds_, short stid_, short edid_, clck s_, clck stt_, clck edt_, int price_, int num_) : 
    saledate(saledate_), name(name_), seatid(seatid_), sts(sts_), eds(eds_), stid(stid_), edid(edid_), s(s_), stt(stt_), edt(edt_), price(price_), num(num_) {};
  void output();
};

struct ticketsystem {
  // <订单号, > - sta，id->订单号
  FileManager<Loginfo> Flog; 
  // 车次 订单号<trainid logid> -> <logid>订单号
  Bptree::BPlusTree<pair<int, pair<int,int>>, pair<int,int>, int> ulog;
  
  // -trainid < -Flog.id, -user.id >
  Bptree::BPlusTree<pair<int, pair<int, int> >, pair<int, int>, int> pnd;
  void exit(){
    ulog.exit();
    pnd.exit();
  }
  void Init(const char* file_name) {
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
  void buy_ticket(const int& uid, Loginfo& info, const bool& type);
  void query_order(const int& uid);
  sjtu::vector<pair<int, int> > refund_ticket(const int& uid, const int& id, int& tid);
};

#endif
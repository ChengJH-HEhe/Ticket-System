#include "ticket.hpp"
#include "config.h"

// buy ticket & refund ticket
// (新到旧 -u -n) -> (旧到新 -i) 
// ulog find -u -n -> loginfo (取消候补？) loginfo status pnd?

// pnd check -trainid all pnding

void ticketsystem::buy_ticket(const int& uid, Loginfo& info, const bool& type, const int& outp = 1) {
  ++Flog.Count;
  if(!type && !info.type) {
      if(outp)
        std::cout << "-1\n";
      return;
  }
  ulog.Insert(pair<int,pair<int,int>>(uid, pair<int,int>(Flog.Count, type)), 
    pair<int,int>(Flog.Count, type));
  Flog.modify_content(info, Flog.Count);
  if(!type) {
    if(outp)std::cout << "queue\n";
    pnd.Insert({info.trainid, {Flog.Count, uid}}, {Flog.Count, uid});
  } else {
    if(outp)std::cout << info.num * info.price << '\n';
  }
}
void ticketsystem::query_order(const int &uid) {
  //std::cerr << uid << std::endl;
  sjtu::vector<pair<int,int> > res;
  ulog.GetValue(uid, &res);
  std::cout << res.size() << std::endl;
  for(int j = int(res.size() - 1); j>=0; --j) {
    auto i = res[j];
    Loginfo info;
    Flog.get_content(info, i.first);
    switch(i.second) {
      case 1: std::cout << "[success] ";break;
      case 0: std::cout << "[pending] ";break;
      case -1: std::cout << "[refunded] ";break;
    }
    std::cout << info.name << ' ' 
    << info.sts << ' ' << info.stt.to_string() << " -> " << info.eds << ' ' << info.edt.to_string() << ' '
    << info.price << ' ' << info.num << '\n';
  }
}
// ulog -> uid id-th ticket, trainid -> tid
sjtu::vector<pair<int, int>> ticketsystem::refund_ticket(const int& uid,const int& id, int& tid) {
  // get uid, Logid
  sjtu::vector< pair<int, int> > res;
  // Trainid -> Logid uid
  sjtu::vector< pair<int, int> > res1;
  ulog.GetValue(uid, &res);
  if(res.size() < id) {
    return std::cout << "-1\n", res1;
  }
  auto wait = res[res.size()-id];
  switch(wait.second) {
    case -1:{
      return std::cout << "-1\n", res1;
    };break;
    case 0: {
      // pending -> refunded
      ulog.update({uid, wait}, {uid, {wait.first, -1}});
      // get trainid
      Loginfo info;
      Flog.get_content(info, wait.first);
      tid = info.trainid;
      //
      pnd.Remove({info.trainid, {wait.first, uid}});
    };break;
    case 1: {
      // success -> refunded
      ulog.update({uid, wait}, {uid, {wait.first, -1}});
      Loginfo info;
      // get vector<>
      Flog.get_content(info, wait.first);
      tid = info.trainid;
      pnd.Remove({info.trainid, {wait.first, uid}});
      pnd.GetValue(info.trainid, &res1);
      res1.push_back({wait.first, uid});
      // update seat?
    };break;
  }
  std::cout << "0\n";
  return res1;
  // check all trainid
}
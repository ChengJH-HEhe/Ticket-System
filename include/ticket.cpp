#include "ticket.hpp"
#include "config.h"
 
#include <sstream>

// buy ticket & refund ticket
// (新到旧 -u -n) -> (旧到新 -i) 
// ulog find -u -n -> loginfo (取消候补？) loginfo status pnd?

// pnd check -trainid all pnding

void ticketsystem::buy_ticket(const int& uid, const Loginfo& info, const bool& type) {
  ++Flog.Count;
  if(!type && !info.type) {
      return std::cout << "-1\n", void();
  }
  ulog.Insert(pair<int,pair<int,int>>(uid, pair<int,int>(Flog.Count, type)), 
    pair<int,int>(Flog.Count, type));
  Flog.modify_content(info, Flog.Count);
  if(!type) {
    std::cout << "queue\n";
    pnd.Insert({info.trainid, Flog.Count}, Flog.Count);
  } else {
    std::cout << info.num * info.price << '\n';
  }
}
void ticketsystem::query_order(std::stringstream& in) {
  int uid;
  in >> uid;
  sjtu::vector<pair<int,int> > res;
  ulog.GetValue(uid, &res);
  std::cout << res.size() << std::endl;
  for(auto i : res) {
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
// ulog uid
sjtu::vector<int> ticketsystem::refund_ticket(const int& uid,const int& id, int& tid) {
  // get loginfo
  sjtu::vector<pair<int,int> > res;
  sjtu::vector<int> res1;
  ulog.GetValue(uid, &res);
  if(res.size() < id) {
    return std::cout << "-1\n", res1;
  }
  switch(res[res.size()-id-1].second) {
    case -1:{
      return std::cout << "-1\n", res1;
    };break;
    case 0: {
      // pending -> refunded
      ulog.update({uid, res[res.size()-id-1]}, {uid, {res[res.size()-id-1].first,-1}});
      // get trainid
      Loginfo info;
      Flog.get_content(info, res[res.size()-id-1].first);
      tid = info.trainid;
      pnd.Remove({info.trainid, res[res.size()-id-1].first});
    };break;
    case 1: {
      // success -> refunded
      ulog.update({uid, res[res.size()-id-1]}, {uid, {res[res.size()-id-1].first,-1}});
      Loginfo info;
      // get vector<>
      Flog.get_content(info, res[res.size()-id-1].first);
      tid = info.trainid;
      pnd.Remove({info.trainid, res[res.size()-id-1].first});
      pnd.GetValue(info.trainid, &res1);
    };break;
  }
  return res1;
  // check all trainid
}
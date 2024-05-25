#include "train.hpp"
#include "config.h"
#include "pair.hpp"
#include <sstream>
#include <string>

// id trainid;
TrainInfo TrainManager::get_one_TrainInfo(const StationT &name, const int &id) {
  // get the traininfo of the station
  TrainInfo res;
  res.id = id;
  seat.GetUniqueValue(name, res); // get unique value
  return res;
}

void Train::Init(std::stringstream &in) {
  // -i -n -m -s -p -x -t -o -d -y
  std::string typ;
  int tp;
  short tmp;
  while (in >> typ) {
    switch (typ[1]) {
    case 'i':
      in >> trainID;
      break;
    case 'n':
      in >> stationNum;
      break;
    case 'm':
      in >> seatNum;
      break;
    case 's':
      in >> typ, Init_string<StationT>(stations, typ);
      break;
    case 'p':
      in >> typ, Init_int(prices, typ);
      break;
    case 'x':
      in >> typ, startTime = calc_minute(typ);
      break;
    case 't':
      in >> typ, Init_short(travelTimes, typ);
      break;
    case 'o':
      in >> typ, Init_short(stopoverTimes, typ);
      break;
    case 'd':
      in >> typ, Init_time(saleDate, typ);
      break;
    case 'y':
      in >> this->type;
      break; 
    default:
      break;
    }
  }
}

void Train::Init_time(short *idx, std::string &input) {
  if (input == "_")
    return;
  size_t cur = 0, nxt;
  while ((nxt = input.find('|', cur)) != std::string::npos) {
    *idx = calc_time(input.substr(cur, nxt - cur));
    ++idx;
    cur = nxt + 1;
  }
  *idx = calc_time(input.substr(cur));
}

void Train::Init_short(short *idx, std::string &input) {
  short tmp = 0;
  for (auto i : input) {
    if (i == '|')
      *idx = tmp, ++idx, tmp = 0;
    else
      tmp = tmp * 10 + i - '0';
  }
  *idx = tmp;
}

void Train::Init_int(int *idx, std::string &input) {
  int tmp = 0;
  for (auto i : input) {
    if (i == '|')
      *idx = tmp, ++idx, tmp = 0;
    else
      tmp = tmp * 10 + i - '0';
  }
  *idx = tmp;
}

void TrainManager::exit() {
  TrainID.exit();
  release.exit();
}

int TrainManager::find_release(const int &id) {
  sjtu::vector<int> result;
  release.GetValue(id, &result);
  return result.empty() ? 0 : result[0];
}

int TrainManager::find_train(const TrainT &cur_Trainname) {
  sjtu::vector<int> result;
  TrainID.GetValue(cur_Trainname, &result);
  return result.empty() ? 0 : result[0];
}
void TrainManager::add_train(std::stringstream &in) {
  Train new_Train;
  new_Train.Init(in);
  if (find_train(new_Train.trainID))
    return std::cout << "-1" << std::endl, void();
  // i ~ i+1 站
  for (int i = 1; i < new_Train.stationNum - 1; ++i) {
    new_Train.prices[i] += new_Train.prices[i - 1];
  }
  ++TrainFile.Count;
  // std::cout << new_Train.trainID << " " << TrainFile.Count << std::endl;
  TrainID.Insert({new_Train.trainID, TrainFile.Count}, TrainFile.Count);
  TrainFile.modify_content(new_Train, TrainFile.Count);
  std::cout << "0" << std::endl;
}

void TrainManager::delete_train(std::stringstream &in) {
  TrainT r;
  std::string type;
  in >> type >> r;
  int id;
  if (!(id = find_train(r)) || release.find(id))
    return std::cout << "-1\n", void();
  TrainID.Remove(pair<TrainT, int>(r, id));
  std::cout << "0\n";
}
void TrainManager::release_train(std::stringstream &in) {
  TrainT r;
  std::string type;
  in >> type >> r;
  int id;
  if (!(id = find_train(r)) || release.find(id))
    return std::cout << "-1\n", void();
  // first part for query seat
  ++ReFile.Count;
  Train old;
  TrainFile.get_content(old, id);
  release.Insert(pair<int, int>(id, ReFile.Count), ReFile.Count);
  int tim = time_distance(old.saleDate[0], old.saleDate[1]) + 1;
  Reinfo *tmp = new Reinfo[tim];
  for (int i = 0; i < tim; ++i)
    tmp[i] = Reinfo(old.seatNum, old.stationNum);
  ReFile.modify_array(tmp, tim, ReFile.Count);
  ReFile.Count += tim;
  // second part update all stations;

  TrainInfo info(r, old.saleDate[0], old.saleDate[1], clck(0, old.startTime), 0,
                 0, 0, -1, 0, id);

  for (int i = 0; i < old.stationNum - 1; ++i) {
    //  std::cout << "Insert " << old.stations[i] << " " << info.stid << " " <<
    //  info.price << " " << info.id << " " << info.starttime << std::endl;
    seat.Insert(pair<StationT, TrainInfo>(old.stations[i], info), info);
    info.price = old.prices[i];
    if (!i)
      info.stoptime = 0;
    info.stid++;
    info.stoptime += (i?old.stopoverTimes[i-1]:0) + old.travelTimes[i];
    info.starttime =
        info.stoptime + (i < old.stationNum - 2 ? old.stopoverTimes[i] : 0);
    // <old.station, T> -> id
  }
  info.starttime = -1;
  info.price = old.prices[old.stationNum - 2];
  seat.Insert(pair<StationT, TrainInfo>(old.stations[old.stationNum - 1], info),
              info);
  std::cout << "0\n";
}
void TrainManager::query_train(std::stringstream &in) {
  TrainT T;
  short time;
  std::string type;
  while (in >> type) {
    switch (type[1]) {
    case 'i':
      in >> T;
      break;
    case 'd':
      in >> type, time = calc_time(type);
      break;
    }
  }
  int id;
  if (!(id = TrainID.find(T)))
    return std::cout << "-1\n", void();

  Train c;
  TrainFile.get_content(c, id);
  if(time < c.saleDate[0] || time > c.saleDate[1])
    return std::cout << "-1\n", void();
  int nid, price = 0;
  // start-time
  short offs = time, offs2;
  clck clk(offs, c.startTime);
  // get seat
  Reinfo nowseat(c.seatNum, c.stationNum);
  if ((nid = release.find(id))) {
    ReFile.get_content(nowseat, nid + time_distance(c.saleDate[0], offs));
  }
  // begin output
  //<stations[i]> <ARRIVING_TIME> -> <LEAVING_TIME> <PRICE> <seat>
  std::cout << T << " " << c.type << '\n'
  << c.stations[0] << " xx-xx xx:xx -> " 
  << clk.to_string() << " 0 " << nowseat.seat[0] << '\n';
  clk.add_forth(c.travelTimes[0]);
  for (int i = 1; i < c.stationNum - 1; ++i) {
    std::cout << c.stations[i] << " "
              << clk.to_string();
    clk.add_forth(c.stopoverTimes[i-1]);
    std::cout << " -> " << clk.to_string() << " " << c.prices[i-1] << " "
              << nowseat.seat[i] << '\n';
    clk.add_forth(c.travelTimes[i]);
  }
  std::cout << c.stations[c.stationNum - 1] << " " << clk.to_string()
            << " -> xx-xx xx:xx " << c.prices[c.stationNum-2] << " x\n";
}

bool cmp_cost(const TrainInfo &a, const TrainInfo &b) {
  if (a.price != b.price)
    return a.price < b.price;
  return a.name < b.name;
};
bool cmp_time(const TrainInfo &a, const TrainInfo &b) {
  if (a.stoptime - a.starttime != b.stoptime - b.starttime)
    return a.stoptime - a.starttime < b.stoptime - b.starttime;
  return a.name < b.name;
};

int TrainManager::get_seat(int index, int st, int ed) {
  Reinfo n;
  ReFile.get_content(n, index);
  int mn = n.seat[st];
  //std::cout << "index" << index << " " << st << " " << ed << std::endl;
  for (int i = st; i < ed; ++i)
    mn = mn < n.seat[i] ? mn : n.seat[i];
    //std::cout << n.seat[i] << " ";
  //std::cout << std::endl;
  return mn;
}
void TrainManager::update_seat(int index, int st, int ed, int num) {
  Reinfo n;
  ReFile.get_content(n, index);
  // if(index == 67) {
  //   //std::cout << std::endl << "indexfirst" << index << ' ' << st << ' ' << ed << ' ' << num << '\n';
  //   for (int i = st; i < ed; ++i)
  //     std::cout << n.seat[i] << ' ';
  //   std::cout << std::endl;
  // }
  for (int i = st; i < ed; ++i)
    n.seat[i] -= num;
  ReFile.modify_content(n, index);
  ReFile.get_content(n, index);
  // if(index == 67) {
  //   std::cout << "indexsecond" << index << ' ' << st << ' ' << ed << '\n';
  //   for (int i = st; i < ed; ++i)
  //     std::cout << n.seat[i] << ' ';
  //   std::cout << std::endl;
  // }
}
/*
  time : time from the beginning (to modify the time)
  date : the date  (to minus the saledate)
  id : seatid not trainid
*/
void TrainManager::print(const TrainInfo &a, clck time, const short &date,
                         const StationT &sts, const StationT &eds) {
  
  std::cout << a.name << " " << sts << " "
            << time.add_forth(a.starttime).to_string() << " -> " << eds << " "
            << time.add_forth(a.stoptime - a.starttime).to_string() << " "
            << a.price << " "
            << get_seat(a.id + time_distance(a.saleDate0, date), a.stid, a.edid)
            << std::endl;
}
void TrainManager::query_ticket(std::stringstream &in) {
  StationT sts, eds;
  std::string type, choice = "time";
  clck time;
  while (in >> type)
    switch (type[1]) {
    case 's':
      in >> sts;
      break;
    case 't':
      in >> eds;
      break;
    case 'd':
      in >> type, time.tim = calc_time(type);
      break;
    case 'p':
      in >> choice;
      break;
    default:
      break;
    }

  sjtu::vector<TrainInfo> st, ed, res;
  seat.GetValue(sts, &st), seat.GetValue(eds, &ed);
  size_t i = 0, j = 0;
  while (i < st.size() && j < ed.size()) {
    while (j < ed.size() && ed[j].id < st[i].id)
      ++j;
    if (j == ed.size())
      break;
    int tmp;
    int tmp_startdate = time.back(st[i].starttime + st[i].st.hm);
    // the day this train start : time.back(st[i].starttime+st[i].st.hm)
    // the first day this train start : st[i].st
    // not in the saledate interval?
    if (st[i].id == ed[j].id && ~st[i].starttime && ~ed[j].stoptime &&
        (tmp = find_release(st[i].id)) &&
        tmp_startdate >= st[i].saleDate0 && tmp_startdate <= st[i].saleDate1)
      res.push_back({st[i].name, st[i].saleDate0, st[i].saleDate1, st[i].st,
                     st[i].stid, ed[j].stid, ed[j].price - st[i].price,
                      ed[j].stoptime, st[i].starttime, tmp});
    ++i;
  }
  std::cout << res.size() << '\n';
  if (res.empty()) {
    return;
  }
  int *xb = new int[res.size()];
  for (int i = 0; i < res.size(); ++i)
    xb[i] = i;
  auto sort = [&](auto self, int l, int r,
                  bool (*cmp)(const TrainInfo &, const TrainInfo &)) {
    // quicksort
    if (l > r)
      return;
    int val = l + rand() % (r - l + 1);
    int *low = xb + l, *high = xb + r;
    while (low < high) {
      while (low < high && cmp(res[val], res[*high]))
        --high;
      while (low < high && cmp(res[*low], res[val]))
        ++low;
      std::swap(*low, *high);
    }
    self(self, l, low - xb - 1, cmp), self(self, low - xb + 1, r, cmp);
  };
  if (choice == "cost")
    sort(sort, 0, res.size() - 1, cmp_cost);
  else
    sort(sort, 0, res.size() - 1, cmp_time);

    // id 不匹配
  for (int i = 0; i < res.size(); ++i) {
    //std::cout << time.tim << " " << res[xb[i]].starttime + res[xb[i]].st.hm << std::endl;
    res[xb[i]].st.tim = time.back(res[xb[i]].starttime + res[xb[i]].st.hm);
    //std::cout << res[xb[i]].st.tim << " " << res[xb[i]].id << " " << res[xb[i]].saleDate0 << std::endl;
    // time  bug..
    print(res[xb[i]], res[xb[i]].st, res[xb[i]].st.tim,
          sts, eds);
  }
  delete[] xb;
}

void TrainManager::query_transfer(std::stringstream &in) {
  StationT sts, eds;
  std::string type, choice = "time";
  clck time;
  while(in >> type)
    switch (type[1]) {
    case 's':
      in >> sts; break;
    case 't':
      in >> eds; break;
    case 'd':
      in >> type, time.tim = calc_time(type); break;
    case 'p':
      in >> choice; break;
    default: break;
    }
  sjtu::vector<TrainInfo> st, ed;
  seat.GetValue(sts, &st), seat.GetValue(eds, &ed);
  // nead to get the real train
  bool tp = type == "time" ? 0 : 1;
  if (st.empty() || ed.empty()) {
    std::cout << "0\n";
    return;
  }
  // all info is needed
  Train *trs = new Train[st.size()], *trt = new Train[ed.size()];
  int *vals = new int[st.size()], *valt = new int[ed.size()], tmpdate;
  for (int i = 0; i < st.size(); ++i)
    if ((tmpdate = time.back(st[i].starttime + (st[i].st.hm))) <=
            st[i].saleDate1 &&
        tmpdate >= st[i].saleDate0 && (vals[i] = find_release(st[i].id)))
      TrainFile.get_content(trs[i], st[i].id);
    else
      vals[i] = 0;
  for (int i = 0; i < ed.size(); ++i)
    if ((valt[i] = find_release(ed[i].id)))
      TrainFile.get_content(trt[i], ed[i].id);
    else
      valt[i] = 0;
  // store a temp : time cost
  TrainInfo ans1, ans2, tmp1, tmp2;
  bool first = 0;
  auto cmp = [&](const bool &type, const TrainInfo &a, const TrainInfo &b,
                 const TrainInfo &c, const TrainInfo &d, const int &inva,
                 const int &invb) {
    if (!first)
      return true;
    if (type) {
      if (a.price + b.price != c.price + d.price)
        return a.price + b.price < c.price + d.price;
      return inva < invb;
    } else {
      if (inva != invb)
        return inva < invb;
      return a.price + b.price < c.price + d.price;
    }
    if (a.name != c.name)
      return a.name < c.name;
    return b.name < d.name;
  };
  int inva, invans = INT_MAX;
  StationT mids;
  short date1, date2;
  auto valid = [&](const int &xb1, const int &xb2) {
    // check if ex a station is the same
    // traininfo: st[i]  ed[j]
    // train      trs[i] trt[j]
    // problem : hard to calc the del time
    int stt = 0, edt;
    for (int i = st[xb1].stid + 1; i < trs[xb1].stationNum; ++i) {
      stt += trs[xb1].travelTimes[i - 1];
      edt = 0;
      for (int j = ed[xb2].stid - 1; j >= 0; --j) {
        edt += trt[xb2].travelTimes[j];
        if (trs[xb1].stations[i] == trt[xb2].stations[j]) {
          // get the inva, invb
          TrainInfo sti = st[xb1], edj = ed[xb2];
          // price time
          sti.price = trs[xb1].prices[i] - trs[xb1].prices[st[xb1].stid];
          edj.price = trt[xb2].prices[ed[xb2].stid] - trt[xb2].prices[j];
          // st start -> st end -> ed start time -> end saledate
          // min saledate
          clck tmped = {trt[xb2].saleDate[0], trt[xb2].startTime};
          tmped.add_forth(ed[xb2].starttime - edt);
          // st:目前是第一站的结尾，稍后改成第一站的起点位置
          // 同时start->stop也要改
          sti.st.add_forth(stt + sti.stoptime);
          sti.st.tim = time.tim;
          if (sti.st.hm > tmped.hm)
            inva = 1440 - minute_distance(tmped.hm, sti.st.hm);
          sti.st.add_forth(inva);
          if (sti.st.tim > trt[xb2].saleDate[1])
            goto pdend;
          else {
            if (sti.st.tim < trt[xb2].saleDate[0])
              inva += int(trt[xb2].saleDate[0] - sti.st.tim) * 1440;
          }
          if (cmp(tp, sti, edj, ans1, ans2, stt + edt + inva, invans)) {
            ans1 = sti, ans2 = edj, invans = inva + stt + edt;
            if (!first)
              first = 1;
            mids = trs[xb1].stations[i];
            // move st back to normal
            ans1.st = {time.back((st[xb1].st.hm) + st[xb1].starttime),
                       st[xb1].st.hm};
            ans2.st = {sti.st.back(ed[xb2].stoptime - edt), ed[xb1].st.hm};
            // update time
            ans1.stoptime = ans1.starttime + stt;
            ans2.starttime = ans2.stoptime - edt;
            // update stationid
            ans1.edid = i, ans2.stid = j;
            date1 = trs[xb1].saleDate[0], date2 = trs[xb2].saleDate[1];
          }
        }
      pdend:;
        edt += (j ? trt[xb2].stopoverTimes[j - 1] : 0);
      }
      stt += trs[xb1].stopoverTimes[i - 1];
    }
  };
  for (int i = 0; i < st.size(); ++i)
    if (vals[i])
      for (int j = 0; j < ed.size(); ++j) {
        // halt for a long time ?
        if (vals[j])
          valid(i, j);
        // for price earlier better
        // for time earlier max(-d, saledatej0+starttime2)
      }
  if (!first)
    std::cout << "0\n";
  else {
    ans1.id = find_release(ans1.id), ans2.id = find_release(ans2.id);
    print(ans1, ans1.st, date1, sts, mids);
    print(ans2, ans2.st, date2, mids, eds);
  }
}
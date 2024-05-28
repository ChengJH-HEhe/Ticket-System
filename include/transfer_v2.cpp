#include "train.hpp"


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
  if(time.tim < 601) {
    std::cout << "0\n";
    return;
  }
  // nead to get the real train
  
  sjtu::vector<TrainInfo> st;
  seat.GetValue(sts, &st);
  if (st.empty()) {
    std::cout << "0\n";
    return;
  }
  int tmpdate;
  // store a temp : time cost
  Train *trs = new Train,*trt = new Train;
  // tmpdate : st[i] start date
  TrainInfo ans1, ans2, tmp1, tmp2;
  bool first = 0;
  // (a, b) try to update (c, d)

  bool tp = choice == "time" ? 0 : 1;
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
  int inva = 0, invans = INT_MAX;
  // mids name
  StationT mids;

/*
st[i]     ->          mid       ->     end
station_start         station_mid
                      station_mid       station_end

st[i] -> enumerate station id;
*/
  auto valid = [&](const int &xb1) {
    // check if ex a station is the same
    // traininfo: st[i]  ed[j]
    // train      trs[i] trt[j]
    /*
      station stid -> i 
      price[stid] to stationid=stid+1

      edt 是第二辆车出发到中转站的时间 

    */
    int stt = 0, edt;
    for (int i = st[xb1].stid + 1; i < trs->stationNum; ++i) {
      stt += trs->travelTimes[i - 1];
      ed
      for (int j = ed[xb2].stid - 1; j >= 0; --j) {
        edt += trt[xb2].travelTimes[j];
        if (trs->stations[i] == trt[xb2].stations[j]) {
          /*
            id = trainid
            get the inva, invb
          */ 
          inva = 0;
          TrainInfo sti = st[xb1], edj = ed[xb2];
          sti.price = trs->prices[i - 1] - (st[xb1].stid?trs->prices[st[xb1].stid-1]:0);
          edj.price = trt[xb2].prices[ed[xb2].stid - 1] - (j?trt[xb2].prices[j - 1]:0);
          // st start -> st end -> ed start time -> end saledate
          // tmped.hm 是第二站的起始时刻
          clck tmped = {trt[xb2].saleDate[0], trt[xb2].startTime};
          tmped.add_forth(ed[xb2].stoptime - edt);
          // sti.st:第一辆车的结束时刻 st.hm 第一辆车的发车时刻
          // 从起点发车 先计算时刻，然后再取时间为现在
          sti.st.tim = 601;
          sti.st.add_forth(sti.starttime);
          sti.st.tim = time.tim;
          sti.st.add_forth(stt);
          // 到站时刻 st.tim st.hm 时刻的一二站间隙
          // 判断此时是否已经开票 但注意此时的日期差距 第二班车起始的 tmped 的日期
          int delta = (sti.st.hm > tmped.hm)? 1440 - minute_distance(tmped.hm, sti.st.hm) 
            : minute_distance(sti.st.hm, tmped.hm);
          inva += delta, sti.st.add_forth(delta);
          if (sti.st.tim > tmped.forth(1440*(int)time_distance(trt[xb2].saleDate[0], trt[xb2].saleDate[1])).tim)
            goto pdend;
          if (sti.st.tim < tmped.tim) {
            int tmp = time_distance(sti.st.tim, tmped.tim); 
            inva += tmp * 1440, sti.st.add_forth(tmp * 1440);
          }
          // sti.st 走到第二辆车的第一站发车
          // inva已经是第一站结束到第二站发车的时间距离了
          // sti.st 是第二站的出发时间
          if (cmp(tp, sti, edj, ans1, ans2, stt + edt + inva, invans)) {
            ans1 = sti, ans2 = edj, invans = inva + stt + edt;
            if (!first)
              first = 1;
            mids = trs->stations[i];
            // 第一辆车的发车日期 + 时刻
            ans1.st = {time.back(st[xb1].st.hm + st[xb1].starttime),
                       st[xb1].st.hm};
            // 当前车次
            ans2.st = {sti.st.back(ed[xb2].stoptime - edt + ed[xb2].st.hm), ed[xb2].st.hm};
            // update time
            ans1.stoptime = ans1.starttime + stt;
            ans2.starttime = ans2.stoptime - edt;
            // update stationid
            ans1.edid = i, ans2.edid = ans2.stid, ans2.stid = j;
          }
        }
      pdend:;
        edt += (j ? trt[xb2].stopoverTimes[j - 1] : 0);
      }
      stt += trs->stopoverTimes[i - 1];
    }
  };
  for (int i = 0; i < st.size(); ++i)
    if ((tmpdate = time.back(st[i].starttime + st[i].st.hm)) <=
            st[i].saleDate1 &&
        tmpdate >= st[i].saleDate0) {
          TrainFile.get_content(*trs, st[i].id);
          valid(i);
    }
  // delete 
  delete trs, delete trt;
  if (!first)
    std::cout << "0\n";
  else {
    ans1.id = find_release(ans1.id), ans2.id = find_release(ans2.id);
    print(ans1, ans1.st, ans1.st.tim, sts, mids);
    print(ans2, ans2.st, ans2.st.tim, mids, eds);
  }
}
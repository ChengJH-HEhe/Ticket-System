
#include "mainsystem.hpp"
#include "config.h"
#include <sstream>
std::string users[] = {"add_user", "login", "logout", "query_profile",
                       "modify_profile"};
std::string trains[] = {"add_train",   "delete_train", "release_train",
                        "query_train", "query_ticket", "query_transfer"};
std::string privates[] = {"buy_ticket", "query_order", "refund_ticket", "clean",
                          "exit"};
// the last functions
// logfile sucfile:indx, pndfile:indx
// traverse all pending file



void mainsystem::buy_ticket(std::stringstream &in) {
  // login
  UserNameT uname;
  int uid;
  std::string type;
  Loginfo ins;
  ins.type = 0;
  while (in >> type) {
    switch (type[1]) {
    case 'u':
      in >> uname;
      break;
    case 'i':
      in >> ins.name;
      break;
    case 'd':
      in >> type, ins.s.tim = calc_time(type);
      break;
    case 'n':
      in >> ins.num;
      break;
    case 'f':
      in >> ins.sts;
      break;
    case 't':
      in >> ins.eds;
      break;
    case 'q':
      in >> type, ins.type = type == "false" ? 0 : 1;
      break;
    default:
      break;
    }
  }
  if (!(uid = userSystem.um.find_user(uname, 1))) {
    std::cout << -1 << '\n';
    return;
  }
  int id1 = trainSystem.TM.find_train(ins.name.hsh()), id2;
  // trainsystem check seat?
  if (!id1 || !(id2 = trainSystem.TM.find_release(id1))) {
    std::cout << "-1" << '\n';
    return;
  }
  // we need to get the traininfo
  TrainInfo infos = trainSystem.get_TrainInfo(ins.sts, id1),
            infot = trainSystem.get_TrainInfo(ins.eds, id1);
  if(infos.id == -1 || infot.id == -1 || infos.stid > infot.stid) {
    std::cout << "-1" << '\n';
    return;
  }
  
  ins.stid = infos.stid, ins.edid = infot.stid; // station-id
  ins.price = infot.price - infos.price; // price
  ins.s.tim = ins.s.back(infos.starttime +  infos.st.hm); // date-back
  if (infos.saleDate1 < ins.s.tim || infos.saleDate0 > ins.s.tim) {
    std::cout << "-1\n";
    return;
  } // not in the interval
  // time & saledate
  ins.s.hm = infos.st.hm;
  ins.stt = ins.s.forth(infos.starttime), ins.edt = ins.s.forth(infot.stoptime);
  ins.saledate = infos.saleDate0;
  ins.seatid = id2 + time_distance(ins.saledate, ins.s.tim);
  // id2 + time_distance(should be correct)
  int res = trainSystem.check(ins.seatid,
                        ins.stid, ins.edid, ins.num);
  if (res < 0)
    std::cout << "-1\n";
  else
    ticketSystem.buy_ticket(uid, ins, res);
}
void mainsystem::query_order(std::stringstream &in) {
  int uid;
  std::string type;
  UserNameT username;
  in >> type >> username;
  uid = userSystem.um.find_user(username, 1);
  if(!uid) {
    std::cout << "-1\n";
  } else {
    ticketSystem.query_order(uid);
  }
}
void mainsystem::refund_ticket(std::stringstream &in) {
  int uid, id = 1;
  UserNameT username;
  std::string type;
  while (in >> type) {
    switch (type[1]) {
    case 'u':
      in >> username;
      break;
    case 'n':
      in >> id;
      break;
    }
  }
  if (!(uid = userSystem.um.find_user(username, 1))) {
    return std::cout << "-1\n", void();
  }
  int seatid;
  // LOGID userid
  sjtu::vector< pair<int,int> > res1 = ticketSystem.refund_ticket(uid, id, seatid);
  if (res1.empty())
    return;
  // update seat for res1.back()
  // update trainsystem 
  // succuss not tui
  Loginfo ins;
  ticketSystem.Flog.get_content(ins, res1.back().first);
  trainSystem.TM.update_seat(seatid, ins.stid, ins.edid, -ins.num);
  for (int i = 0; i < int(res1.size()) - 1; ++i) {
    ticketSystem.Flog.get_content(ins, res1[i].first);
      // queue -> success
      //ins.output();
      if (trainSystem.check(seatid,
                          ins.stid, ins.edid, ins.num) > 0) {
      // ulog update : 0 -> 1
      // user[id] pnd. -> userid: 
        ticketSystem.ulog.update({res1[i].second, {res1[i].first, 0}},
        {res1[i].second, {res1[i].first, 1}});
        //ins.output();
        ticketSystem.pnd.Remove({seatid, res1[i]});
      }
  }
}
void mainsystem::clean() {
  std::cout << "0\n";
}
void mainsystem::exit() {
  std::cout << "bye\n";
  userSystem.um.loginUser.exit(1);
}
Timer query_transfer_timer("query_transfer");

bool mainsystem::init(std::stringstream &in) {
  std::string tim;
  in >> tim;
  std::string cmd;
  std::cout << tim << " ";
  in >> cmd;
  for (int i = 0; i < 5; ++i) {
    if (users[i] == cmd) {
      switch (i) {
      case 0:
        userSystem.add_user(in);
        break;
      case 1:
        userSystem.login(in);
        break;
      case 2:
        userSystem.logout(in);
        break;
      case 3:
        userSystem.query_profile(in);
        break;
      case 4:
        userSystem.modify_profile(in);
        break;
      default:
        break;
      }
      return 1;
    }
  }
  for (int i = 0; i < 6; ++i) {
    if (trains[i] == cmd) {
      switch (i) {
      case 0:
        trainSystem.add_train(in);
        break;
      case 1:
        trainSystem.delete_train(in);
        break;
      case 2:
        trainSystem.release_train(in);
        break;
      case 3:
        trainSystem.query_train(in);
        break;
      case 4:{
        trainSystem.query_ticket(in);
      }
        break;
      case 5:{
        query_transfer_timer.start();
        trainSystem.query_transfer(in);
        query_transfer_timer.stop();
      }
        break;
      }
      return 1;
    }
  }
  for (int i = 0; i < 5; ++i)
    if (privates[i] == cmd) {
      switch (i) {
      case 0:
        buy_ticket(in);
        break;
      case 1:
        query_order(in);
        break;
      case 2:
        refund_ticket(in);
        break;
      case 3:
        clean();
        break;
      case 4: {
        exit();
        return 0;
      }
        break;
      }
      return 1;
    }
  return 1;
}

#include "mainsystem.hpp"
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
  in >> type;
  in >> uname;
  if (!(uid = userSystem.um.find_user(uname, 1))) {
    std::cout << -1 << std::endl;
    return;
  }
  Loginfo ins;
  while (in >> type) {
    switch (type[1]) {
    case 'i':
      in >> ins.name;
      break;
    case 'd':
      in >> ins.s.tim;
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

  int id1 = trainSystem.TM.find_train(ins.name), id2;
  // trainsystem check seat?
  if (!id1 || !(id2 = trainSystem.TM.find_release(id1))) {
    std::cout << "-1" << std::endl;
    return;
  }
  ins.trainid = id1;
  // we need to get the traininfo
  TrainInfo infos = trainSystem.get_TrainInfo(ins.sts, id1),
            infot = trainSystem.get_TrainInfo(ins.eds, id1);
  ins.stid = infos.stid, ins.edid = infot.stid;
  ins.price = infot.price - infos.price;
  ins.s.tim = ins.s.back(infos.starttime + infos.st.hm);
  if (infos.saleDate1 < ins.s.tim || infos.saleDate0 > ins.s.tim) {
    std::cout << "-1" << std::endl;
    return;
  }
  ins.stt = ins.s.forth(infos.starttime), ins.edt = ins.s.forth(infot.stoptime);
  ins.s.hm = infos.st.hm;
  ins.saledate = infos.saleDate0;
  // id1 + time_distance(should be correct)
  if (trainSystem.check(id1 + time_distance(infos.saleDate0, ins.s.tim),
                        ins.stid, ins.edid, ins.num))
    ticketSystem.buy_ticket(uid, ins, 1),
        trainSystem.TM.update_seat(id1, ins.stid, ins.edid, ins.num);
  else
    ticketSystem.buy_ticket(uid, ins, 0);
}
void mainsystem::query_order(std::stringstream &in) {
  ticketSystem.query_order(in);
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
    case 'i':
      in >> id;
      break;
    }
  }
  if (!(uid = userSystem.um.find_user(username, 1))) {
    return std::cout << "-1\n", void();
  }
  int tid;
  sjtu::vector<int> res1 = ticketSystem.refund_ticket(uid, id, tid);
  if (res1.empty())
    return;
  for (int i = res1.size() - 1; i >= 0; --i) {
    Loginfo ins;
    ticketSystem.Flog.get_content(ins, res1[i]);
    if (trainSystem.check(tid + time_distance(ins.saledate, ins.s.tim),
                          ins.stid, ins.edid, ins.num)) {
      trainSystem.TM.update_seat(tid, ins.stid, ins.edid, -ins.num);
      ticketSystem.buy_ticket(uid, ins, 1);
      ticketSystem.pnd.Remove({tid, res1[i]});
    }
  }
}
void mainsystem::clean() {
  userSystem.remove();
  trainSystem.remove();
  ticketSystem.remove();
}
void mainsystem::exit() {
  userSystem.remove(1);
  std::cout << "bye\n";
}

void mainsystem::init(std::stringstream &in) {
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
      break;
    }
  }
  for (int i = 0; i < 5; ++i) {
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
      case 4:
        trainSystem.query_ticket(in);
        break;
      case 5:
        trainSystem.query_transfer(in);
        break;
      default:
        break;
      }
      break;
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
      case 4:
        exit();
        break;
      }
      break;
    }
}
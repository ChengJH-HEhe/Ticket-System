#ifndef MAINSYSTEM_HPP
#define MAINSYSTEM_HPP
#include "ticket.hpp"
#include "trainsystem.hpp"
#include "usersystem.hpp"
#include <sstream>
class mainsystem {
private:
  usersystem userSystem;
  trainsystem trainSystem;
  ticketsystem ticketSystem;
  void buy_ticket(std::stringstream&);
  void query_order(std::stringstream&);
  void refund_ticket(std::stringstream&);
  void clean();
  void exit();
public:
  // 传进一个 stringstream 然后开始分系统
  
  void exi() {
    userSystem.exit();
    trainSystem.exit();
    ticketSystem.exit();
  }
  mainsystem() {
    userSystem.Init("User");
    trainSystem.Init("Train");
    ticketSystem.Init("Ticket");
  }
  bool init(std::stringstream &in);
};

#endif
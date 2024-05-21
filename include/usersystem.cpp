#include "usersystem.hpp"
#include "config.h"

void usersystem::add_user(std::stringstream& in) {
  std::string type;
  // -p -n -m -g
  UserNameT cur, now;
  PassT password;
  NameT name;
  MailT email;
  int tp;
  in >> cur >> now;
  while(in>>type) {
    switch(type[1]){
      case 'p':in>>password;break;
      case 'n':in>>name;break;
      case 'm':in>>email;break;
      case 'g':in>>tp;break;
      default:break;
    }
  }
  std::cout << um.add_user(cur, now, password, name, email, tp) << std::endl;
}
void usersystem::login(std::stringstream& in) {
  UserNameT cur;
  PassT password;
  in>>cur>>password;
  std::cout << um.login(cur, password) << std::endl;
}
void usersystem::logout(std::stringstream& in) {
  UserNameT cur;
  in >> cur;
  std::cout << um.logout(cur) << std::endl;
}
void usersystem::query_profile(std::stringstream& in) {
  UserNameT cur,user;
  in>>cur>>user;
  std::cout << um.query_profile(cur, user) << std::endl;
}
void usersystem::modify_profile(std::stringstream& in) {
  std::string type;
  UserNameT cur, now;
  PassT password;
  NameT name;
  MailT email;
  int tp;
  in >> cur >> now;
  while(in>>type) {
    switch(type[1]){
      case 'p':in>>password;break;
      case 'n':in>>name;break;
      case 'm':in>>email;break;
      case 'g':in>>tp;break;
      default:break;
    }
  }
  std::cout << um.modify_profile(cur, now, password, name, email, tp) << std::endl;
}
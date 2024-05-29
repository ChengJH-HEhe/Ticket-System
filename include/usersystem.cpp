#include "usersystem.hpp"
#include "config.h"
#include <sstream>


void usersystem::add_user(std::stringstream& in) {
  std::string type;
  // -p -n -m -g
  UserNameT cur, now;
  PassT password;
  NameT name;
  MailT email;
  int tp;
  while(in>>type) {
    switch(type[1]){
      case 'c':in >> cur;break;
      case 'u':in >> now; break;
      case 'p':in>>password;break;
      case 'n':in>>name;break;
      case 'm':in>>email;break;
      case 'g':in>>tp;break;
      default:break;
    }
  }
  std::cout << um.add_user(cur, now, password, name, email, tp) << '\n';
}
void usersystem::login(std::stringstream& in) {
  UserNameT cur;
  PassT password;
  std::string type;
  while(in>>type) {
    switch(type[1]){
      case 'u':in >> cur;break;
      case 'p':in>>password;break;
    }
  }
  std::cout << um.login(cur, password) << '\n';
}
void usersystem::logout(std::stringstream& in) {
  UserNameT cur;
  std::string type;
  while(in>>type) {
    switch(type[1]){
      case 'u':in >> cur;break;
    }
  }
  std::cout << um.logout(cur) << '\n';
}
void usersystem::query_profile(std::stringstream& in) {
  UserNameT cur,user;
  std::string type;
  // -p -n -m -g
  while(in>>type) {
    switch(type[1]){
      case 'c':in >> cur;break;
      case 'u':in >> user; break;
    }
  }
  std::cout << um.query_profile(cur, user) << '\n';
}
void usersystem::modify_profile(std::stringstream& in) {
  std::string type;
  UserNameT cur, now;
  PassT password;
  NameT name;
  MailT email;
  int tp = -1;
  while(in>>type) {
    switch(type[1]){
      case 'c':in >> cur;break;
      case 'u':in >> now; break;
      case 'p':in>>password;break;
      case 'n':in>>name;break;
      case 'm':in>>email;break;
      case 'g':in>>tp;break;
      default:break;
    }
  }
  std::cout << um.modify_profile(cur, now, password, name, email, tp) << '\n';
}
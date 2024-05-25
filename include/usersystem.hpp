#ifndef USERSYSTEM_HPP
#define USERSYSTEM_HPP

#include "user.hpp"

struct usersystem {
  UserManager um;
  void remove(int type = 0) {
    um.remove(type);
  }
  void exit() {
    um.exit();
  }
  void Init(const char* file_name) { um.Init(file_name); }
  void add_user(std::stringstream& in);
  void login(std::stringstream& in);
  void logout(std::stringstream& in);
  void query_profile(std::stringstream& in);
  void modify_profile(std::stringstream& in);
};

#endif
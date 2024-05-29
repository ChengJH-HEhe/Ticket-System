#ifndef USER_HPP
#define USER_HPP

#include "bplustree.hpp"
#include "config.h"

struct User{
  User() = default;
  User(const UserNameT& username_, const PassT& password_, const NameT& name_, const MailT& email_, const int& type_) 
  : username(username_), password(password_), name(name_), email(email_), type(type_) {};
  UserNameT username;
  PassT password;
  NameT name;
  MailT email;
  int type;
  std::string profile();
};

struct UserManager {
  // give next page_id
  FileManager<User> UserFile;
  // UserName -> unique id
  Bptree::BPlusTree<pair<unsigned int,int> , int, unsigned int> user, loginUser;
  void exit() {
    user.exit();
    loginUser.exit();
  }
  void remove(int type = 0) {
    if(!type) {
      UserFile.remove();
      user.remove(), loginUser.remove();
    } else {
      loginUser.remove();
    }
  }
  void Init(std::string file_name) {
    UserFile.Init(file_name  + "_ID");
    user.Init(file_name);
    loginUser.Init(file_name + "_ID_Log");
  }
void get_user(User& user, int UserID); 
void modify_user(User& user, int UserID);
int find_user(const UserNameT& cur_username, int type);
int direct_find_user(const UserNameT& cur_username, User& OldUser, bool old);
int add_user(const UserNameT& cur_username, const UserNameT& username, 
const PassT& password,
 const NameT& name, 
 const MailT& email, int type);

int login(const UserNameT& username, const PassT& password);

int logout(const UserNameT& username);

std::string Query_profile(User&, User&, int&, const UserNameT&, const UserNameT&);
std::string query_profile(const UserNameT&, const UserNameT&); 
std::string modify_profile(const UserNameT &, const UserNameT &, const PassT&,
  const NameT& , 
  const MailT& , int );
};
#endif
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
  int UserCount;
#define offset sizeof(UserCount)
  std::fstream User_filestream;
  std::string User_file_name;
  // UserName -> unique id
  Bptree::BPlusTree<UserNameT , int, UserNameT> user, loginUser;
  void Init(std::string file_name) {
    User_file_name = file_name  + "_ID";
    user.Init(file_name);
    loginUser.Init(User_file_name + "_Log");
    User_filestream.open(User_file_name,
                            std::ios::binary | std::ios::in | std::ios::out);
    if (!User_filestream.good()) {
      User_filestream.clear();
      User_filestream.open(User_file_name, std::ios::binary | std::ios::trunc |
                                             std::ios::out | std::ios::in);
      UserCount = 0;
    } else {
      User_filestream.read(reinterpret_cast<char *>(&UserCount),
                              sizeof(int));
    }
  }
  ~UserManager() {
    // std::cerr << "write " << UserCount << std::endl;
    User_filestream.seekp(0);
    User_filestream.write(reinterpret_cast<const char *>(&UserCount),
                             sizeof(int));
    User_filestream.close();
  }
  int GetVacancy() { return ++UserCount; }
  void recycle(int pg) {}
void get_user(User& user, int UserID); 
void modify_user(User& user, int UserID);
int find_user(const UserNameT& cur_username, int type);
bool direct_find_user(const UserNameT& cur_username, User& OldUser, bool old);
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
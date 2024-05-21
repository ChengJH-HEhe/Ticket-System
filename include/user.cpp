#include "user.hpp"
#include "config.h"
#include "vector.hpp"
#include <string>

std::string User::profile() {
  return username.to_string() + " " + name.to_string() + " " + email.to_string() +  " " + std::to_string(type);
}

int UserManager::find_user(const UserNameT& cur_username, int type = 0) {
  sjtu::vector<int> result;
  if(!type)
    user.GetValue(cur_username, &result);
  else 
    loginUser.GetValue(cur_username, &result);
  return result.empty() ? -1 : result[0];
}
void UserManager::get_user(User& olduser, int UserID) {
  User_filestream.seekg(offset + (UserID - 1) * sizeof(User));
  User_filestream.read(reinterpret_cast<char *>(&olduser), sizeof(User)); 
}
void UserManager::modify_user(User& olduser, int UserID) {
  User_filestream.seekp(offset + (UserID - 1) * sizeof(User));
  User_filestream.write(reinterpret_cast<const char *>(&olduser), sizeof(User)); 
}

bool UserManager::direct_find_user(const UserNameT& cur_username, User& OldUser, bool old) {
  int olduserID = find_user(cur_username,old);
  if(olduserID < 0) return 0;
  get_user(OldUser, olduserID);
  return 1;
}

int UserManager::add_user(const UserNameT& cur_username, const UserNameT& username, 
  const PassT& password,
  const NameT& name, 
  const MailT& email, int type) {
  if(!UserCount) type = 10;
  if(cur_username.size()) {
    User OldUser;
    if(!direct_find_user(cur_username, OldUser, 1) || OldUser.type <= type)
      return -1;
  }
  User newuser(username, password, name, email, type);
  ++UserCount;
  user.Insert(username, UserCount);
  return UserCount;
 }

int UserManager::login(const UserNameT& username, const PassT& password) {
  User CurUser;
  int id;
  if(direct_find_user(username, CurUser, 1))
    return -1;
  if(!(id = direct_find_user(username, CurUser, 0)))
    return -1;
  if(!(CurUser.password == password))
    return -1;
  loginUser.Insert(username, id);
  return 0;
}

int UserManager::logout(const UserNameT& username) {
  User CurUser;
  int id = direct_find_user(username, CurUser, 1);
  if(id > 0) 
     return loginUser.Remove(username),0;
  else return -1;
}

std::string UserManager::query_profile(const UserNameT& cur_username, const UserNameT& username) {
  int Nid;
  User CurUser,  NewUser;
  int Cid = direct_find_user(cur_username, CurUser, 1);
  if(Cid < 0) return "-1";
  Cid = CurUser.type;
  Nid = direct_find_user(username, NewUser, 0);
  if(Nid > 0 && NewUser.type <= Cid) 
    return NewUser.profile();
  else return "-1";
}

std::string UserManager::Query_profile(User& CurUser, User& NewUser, int& Nid, const UserNameT& cur_username, const UserNameT& username) {
  int Cid = direct_find_user(cur_username, CurUser, 1);
  if(Cid < 0) return "-1";
  Cid = CurUser.type;
  Nid = direct_find_user(username, NewUser, 0);
  if(Nid > 0 && NewUser.type <= Cid) 
    return NewUser.profile();
  else return "-1";
}

std::string UserManager::modify_profile(const UserNameT &cur_username, const UserNameT &username, const PassT& password,
  const NameT& name, 
  const MailT& email, int type) {
  User CurUser, NewUser;
  int NewId;
  if(Query_profile(CurUser, NewUser, NewId, cur_username, username) == "-1")
    return "-1";
  if(type >= 0){
    if(CurUser.type < type) return "-1";
    NewUser.type = type;
  } 
  if(!name.empty()) {
    NewUser.name = name;
  }
  if(!email.empty()) {
    NewUser.email = email;
  }
  if(!password.empty()) {
    NewUser.password = password;
  }
  modify_user(NewUser, NewId);
  return NewUser.profile();
}

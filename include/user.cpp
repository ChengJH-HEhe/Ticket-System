#include "user.hpp"
#include "config.h"
#include <string>

std::string User::profile() {
  return username.to_string() + " " + name.to_string() + " " + email.to_string() +  " " + std::to_string(type);
}

int UserManager::find_user(const UserNameT& cur_username, int type = 0) {
  sjtu::vector<int> res;
  if(!type)
    user.GetValue(cur_username.hsh(), &res); // user.GetOneValue(cur_username.hsh(), res1);
  else 
    loginUser.GetValue(cur_username.hsh(), &res); // loginUser.GetOneValue(cur_username.hsh(), res1);
  return res.empty()? 0 : res[0];
}
void UserManager::get_user(User& olduser, int UserID) {
  UserFile.get_content(olduser, UserID);
}
void UserManager::modify_user(User& olduser, int UserID) {
  UserFile.modify_content(olduser, UserID);
}

int UserManager::direct_find_user(const UserNameT& cur_username, User& OldUser, bool old) {
  int olduserID = find_user(cur_username,old);
  if(!olduserID) return 0;
  get_user(OldUser, olduserID);
  return olduserID;
}
int UserManager::add_user(const UserNameT& cur_username, const UserNameT& username, 
  const PassT& password,
  const NameT& name, 
  const MailT& email, int type) {
    //std::cout << "add " << username << " " << UserFile.Count << '\n';
  if(!UserFile.Count) type = 10;
  else if(cur_username.size()) {
    User OldUser;
    if(!direct_find_user(cur_username, OldUser, 1) || OldUser.type <= type)
      return -1;
  }
  User newuser(username, password, name, email, type);
  //std::cout<<newuser.name << '\n';
  ++UserFile.Count;
  UserFile.modify_content(newuser, UserFile.Count);
  user.Insert({username.hsh(),UserFile.Count}, UserFile.Count);
  //std::cout << "username " << UserFile.Count << '\n';
  return 0;
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
  //std::cout << username << " login as " << id << '\n';
  loginUser.Insert({username.hsh(),id}, id);
  return 0;
}
int UserManager::logout(const UserNameT& username) {
  User CurUser;
  int id = direct_find_user(username, CurUser, 1);
  if(id > 0) 
     return loginUser.Remove({username.hsh(),id}),0;
  else return -1;
}
std::string UserManager::query_profile(const UserNameT& cur_username, const UserNameT& username) {
  int Nid;
  User CurUser,  NewUser;
  int Cid = direct_find_user(cur_username, CurUser, 1);
  //std::cout << "find " << cur_username << " " << Cid << " " << '\n';
  if(!Cid) return "-1";
  Cid = CurUser.type;
  if(cur_username == username)
    return CurUser.profile();
  Nid = direct_find_user(username, NewUser, 0);
  //std::cout << "find " << username << " " << Nid << " " << NewUser.type << " " << '\n';
  if(Nid > 0 && NewUser.type < Cid) 
    return NewUser.profile();
  else return "-1";
}

std::string UserManager::Query_profile(User& CurUser, User& NewUser, int& Nid, const UserNameT& cur_username, const UserNameT& username) {
  int Cid = direct_find_user(cur_username, CurUser, 1);
  if(!Cid) return "-1";
  Cid = CurUser.type;
  Nid = direct_find_user(username, NewUser, 0);
  if(cur_username == username)
    return "";
  if(Nid > 0 && NewUser.type < Cid) 
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
    if(CurUser.type <= type) return "-1";
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

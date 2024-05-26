#include "trainsystem.hpp"

TrainInfo trainsystem::get_TrainInfo(const StationT& str, const int& id) {
  // 双关键？price排序 
  // 
  return TM.get_one_TrainInfo(str, id);
}

int trainsystem::check(const int& id1, const int& stid, const int&edid, const int&num) {
  return TM.update_seat(id1, stid, edid, num);
}
void trainsystem::add_train(std::stringstream& in) {
  TM.add_train(in);
}
void trainsystem::delete_train(std::stringstream& in) {
  TM.delete_train(in);
}
void trainsystem::release_train(std::stringstream& in) {
  TM.release_train(in);
}
void trainsystem::query_train(std::stringstream& in) {
  TM.query_train(in);
}
void trainsystem::query_ticket(std::stringstream& in) {
  TM.query_ticket(in);
}
void trainsystem::query_transfer(std::stringstream& in) {
  TM.query_transfer(in);
}
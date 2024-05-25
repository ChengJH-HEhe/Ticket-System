#ifndef CONFIG_H
#define CONFIG_H

#include "string.hpp"
#include <fstream>
#include <string>
using UserNameT = sjtu::string<20>;
using PassT = sjtu::string<30>;
using NameT = sjtu::string<15>;
using MailT = sjtu::string<30>;

using TrainT = sjtu::string<20>;
using StationT = sjtu::string<30>;

template <typename T> struct FileManager {
#define ofset 4
  std::fstream filestream;
  std::string file_name;
  int Count;
  void Init(std::string file_name) {
    file_name = file_name;
    filestream.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    if (!filestream.good()) {
      filestream.clear();
      filestream.open(file_name, std::ios::binary | std::ios::trunc |
                                     std::ios::out | std::ios::in);
      Count = 0;
    } else {
      filestream.read(reinterpret_cast<char *>(&Count), sizeof(int));
    }
  }
  void remove() {
    filestream.close();
    std::remove(file_name.c_str());
  }
  ~FileManager() {
    // std::cerr << "write " << UserCount << std::endl;
    filestream.seekp(0);
    filestream.write(reinterpret_cast<const char *>(&Count), sizeof(int));
    filestream.close();
  }
  void get_content(T &user, int UserID) {
    filestream.seekg(ofset + (UserID - 1) * sizeof(T));
    filestream.read(reinterpret_cast<char *>(&user), sizeof(T));
  }
  // 只写一个会不会太浪费？
  void modify_content(const T &user, int UserID) {
    filestream.seekp(ofset + (UserID - 1) * sizeof(T));
    filestream.write(reinterpret_cast<const char *>(&user), sizeof(T));
  }
  void modify_array(T *arr, int size, int UserID) {
    filestream.seekp(ofset + (UserID - 1) * sizeof(T));
    filestream.write(reinterpret_cast<const char *>(arr), size * sizeof(T));
  }
};

template <typename T> void sort(T *a, T *b, bool (*)(const T &a, const T &b)) {}

inline short time_distance(const short &a, const short &b) {
  int days[] = {0, 30, 61};
  return (days[b / 100 - 6] - days[a / 100 - 6]) + (b % 100 - a % 100);
}

inline short minute_distance(const short &a, const short &b) {
  return (b / 100 - a / 100) * 60 + b % 100 - a % 100;
}

struct clck {
  short tim, hm;
  clck(short rtim = 0, short hm_ = 0) : tim(rtim), hm(hm_) {}
  void init(short rtim = 0, short hm_ = 0) { tim = rtim, hm = hm_; }
  std::string to_string() {
    std::string res = "0";
    res += std::to_string(tim / 100) + "-" + std::to_string(tim % 100) + " ";
    if (hm / 100 < 10) {
      res.append("0");
      res.push_back('0' + hm / 100);
    } else
      res.append(std::to_string(hm / 100));
    res.append(":");
    if (hm % 100 < 10) {
      res.append("0");
      res.push_back('0' + hm % 100);
    } else
      res.append(std::to_string(hm % 100));
    return res;
  }
  clck forth(int add) {
    clck tmp(*this);
    return tmp.add_forth(add);
  }
  clck &add_forth(int add) {
    short newm = hm % 100 + add;
    hm -= hm % 100;
    hm /= 100; // only hours
    hm += newm / 60, newm %= 60;
    int lim[3] = {30, 31, 31};
    short newh = hm / 24;
    hm %= 24; hm *= 100; hm += newm;// back h & m
    tim += newh; // most 3 days
    if (tim % 100 > lim[tim / 100 - 6])
      tim = (tim / 100 + 1) * 100 + (tim % 100 - lim[tim / 100 - 6]);
    return *this;
  }
  // judge which day is the startdate
  short back(int del) {
    auto rem = del / 1440;
    int lim[] = {31, 30, 31, 31};
    short tim1 = tim;
    while (rem >= tim1 % 100) {
      tim1 = tim1 / 100 * 100 - 100 + lim[tim1 / 100 - 6];
      rem -= tim1 % 100;
      if (tim1 < 601)
        return -1;
    }
    return tim1 - rem;
  }
};

#endif
#ifndef CONFIG_H
#define CONFIG_H

#include "string.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>

using UserNameT = sjtu::string<20>;
using PassT = sjtu::string<30>;
using NameT = sjtu::string<15>;
using MailT = sjtu::string<30>;

using TrainT = sjtu::string<20>;
using StationT = sjtu::string<30>;

class Timer {
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    long totduration;
  public:
    const char *name;
    Timer() = default;
    Timer(const char *s) : name(s), totduration(0) {}
    void start() {
        start_time = std::chrono::system_clock::now();
    }

    void stop() {
        auto end_time = std::chrono::system_clock::now();
        totduration += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    }

    double duration() {
        return (double)totduration * std::chrono::nanoseconds::period::num / std::chrono::nanoseconds::period::den;
    }

    ~Timer() {
        fprintf(stderr, "%s took %.6lf seconds\n", name, duration());
    }
};

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
      filestream.seekg(0);
      filestream.read(reinterpret_cast<char *>(&Count), sizeof(int));
    }
  }
  void remove() {
    filestream.close();
    Count = -1;
    std::remove(file_name.c_str());
  }
  ~FileManager() {
    // std::cerr << "write " << UserCount << std::endl;
    if(Count < 0) return; 
    filestream.seekp(0);
    filestream.write(reinterpret_cast<const char *>(&Count), sizeof(int));
    filestream.close();
  }
  void get_content(T &user, int UserID) {
    //std::cout << "USERID" << UserID << std::endl;
    filestream.seekg(ofset + (UserID - 1) * sizeof(T));
    filestream.read(reinterpret_cast<char *>(&user), sizeof(T));
  }
  void modify_content(T &user, int UserID) {
    filestream.seekp(ofset + (UserID - 1) * sizeof(T));
    filestream.write(reinterpret_cast<const char *>(&user), sizeof(T));
  }
  // 只写一个会不会太浪费？
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
  return b - a;
}


inline short calc_minute(const std::string &input) {
  return (input[0]) * 600 + (input[1]) * 60 + (input[3]) * 10 + (input[4]) -
         '0' * 671;
}

inline short calc_time(const std::string &input) {
  return (input[0]) * 1000 + (input[1]) * 100 + (input[3]) * 10 + (input[4]) -
         '0' * 1111;
}

struct clck {
  short tim, hm;
  clck(short rtim = 0, short hm_ = 0) : tim(rtim), hm(hm_) {}
  void init(short rtim = 0, short hm_ = 0) { tim = rtim, hm = hm_; }
  std::string to_string() {
    std::string res = "0";
    res += std::to_string(tim / 100) + "-" + (tim%100<10?"0":"") + std::to_string(tim % 100) + " ";
    if (hm / 60 < 10) {
      res.append("0");
      res.push_back('0' + hm / 60);
    } else
      res.append(std::to_string(hm / 60));
    res.append(":");
    if (hm % 60 < 10) {
      res.append("0");
      res.push_back('0' + hm % 60);
    } else
      res.append(std::to_string(hm % 60));
    return res;
  }
  clck forth(int add) {
    clck tmp(*this);
    return tmp.add_forth(add);
  }
  clck &add_forth(int add) {
    int newm = hm % 60 + add;
    hm -= hm % 60;
    hm /= 60; // only hours
    hm += newm / 60, newm %= 60;
    int lim[] = {30, 31, 31, 30, 31, 30, 31};
    int newh = hm / 24;
    hm %= 24; hm *= 60; hm += newm;// back h & m
    tim += newh; // 
    // 2months?
    while (tim % 100 > lim[tim / 100 - 6])
      tim = (tim / 100 + 1) * 100 + (tim % 100 - lim[tim / 100 - 6]);
    return *this;
  }
  // judge which day is the startdate
  short back(int del) {
    auto rem = del / 1440;
    int lim[] = {31, 30, 31, 31, 30, 31, 30, 31};
    short tim1 = tim;
    // rem 这么多天 801 - 1 - 100 + day[7] : 7/31
    while (rem >= tim1 % 100) {
      rem -= tim1 % 100;
      tim1 = tim1 / 100 * 100 - 100 + lim[tim1 / 100 - 6];
      if (tim1 < 601)
        return -1;
    }
    return tim1 - rem;
  }
};

#endif
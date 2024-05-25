#ifndef PAIR_HPP
#define PAIR_HPP

#include <utility>

template <typename T1, typename T2> class pair {
public:
  T1 first;
  T2 second;
  pair() = default;
  pair(T1 first_, T2 second_)
      : first(std::move(first_)), second(std::move(second_)) {}
  bool operator<(const pair<T1, T2> &rhs) const {
    return first < rhs.first || (first == rhs.first && second < rhs.second);
  }
  bool operator==(const pair<T1, T2> &rhs) const {
    return first == rhs.first && second == rhs.second;
  }
  pair<T1,T2>& operator=(const pair<T1,T2>& rhs) {
    first = rhs.first, second = rhs.second;
    return *this;
  }
};
#endif
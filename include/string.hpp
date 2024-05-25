#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <cstring>
#include <string.h>
namespace sjtu {

template<int len = 65>
struct string {
  char s[len + 1];
  string() { s[0] = '\0'; }
  std::string to_string() {
    std::string tmp;
    for (int i = 0; s[i]; ++i)
      tmp.push_back(s[i]);
    return tmp;
  }
  void Init(const std::string& rhs) {
    for(int i = 0; i < rhs.size(); ++i)
      s[i] = rhs[i];
    s[rhs.size()]='\0';
  }
  string(const char *str) {
    strncpy(s, str, sizeof(s) - 1);
    s[sizeof(s) - 1] = '\0';
  }

  string(const string &other) {
    strncpy(s, other.s, sizeof(s) - 1);
    s[sizeof(s) - 1] = '\0';
  }

  string &operator=(const string &other) {
    if (this == &other)
      return *this;
    strncpy(s, other.s, sizeof(s) - 1);
    s[sizeof(s) - 1] = '\0';
    return *this;
  }

  string &operator+=(const string &other) {
    strncat(s, other.s, sizeof(s) - strlen(s) - 1);
    s[sizeof(s) - 1] = '\0';
    return *this;
  }

  string operator+(const string &other) const {
    string result(*this);
    result += other;
    return result;
  }

  bool operator==(const string &other) const { return strcmp(s, other.s) == 0; }
  bool operator<(const string &other) const { return strcmp(s, other.s) < 0; }
  bool operator<=(const string &other) const { return strcmp(s, other.s) <= 0; }
  char &operator[](size_t pos) { return s[pos]; }

  const char &operator[](size_t pos) const { return s[pos]; }

  const char *c_str() const { return s; }

  size_t size() const { return strlen(s); }

  bool empty() const { return size() == 0; }

  void clear() { s[0] = '\0'; }
};
template<int len>
  std::ostream &operator<<(std::ostream &out,const string<len> &b) {
    out << b.s;
    return out;
  }
template<int len>
  std::istream &operator>>(std::istream &in, string<len> &b) {
    in >> b.s;
    return in;
  }
}

#endif
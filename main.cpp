#include "mainsystem.hpp"
#include <sstream>
int main() {
  // std::string str;
  // std::cin >> str;
  // freopen(str.c_str(),"r",stdin);
  // freopen("out.txt","w",stdout);
  mainsystem ms;
  std::string line;
  srand(time(0));
  std::ios::sync_with_stdio(0);
  std::cin.tie(0), std::cout.tie(0);
  while(getline(std::cin, line)) {
    std::stringstream ins(line);
    if(!ms.init(ins))
      break;
  }
  ms.exi();
  return 0;
}


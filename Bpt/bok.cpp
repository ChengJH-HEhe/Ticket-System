#include <cstring>
#include <fstream>
#include <iostream>
#include <cassert>
#include <time.h>
#include <unistd.h>
using std::cin, std::cout;
using std::fstream;
using std::pair;
using std::string;
typedef long long ll;
int _;
template <class T, int info_len = 0> class MemoryRiver {
private:
  /* your code here */
  string file_name;
  fstream file;
  int sizeofT = sizeof(T);

public:
  MemoryRiver() = default;

  MemoryRiver(const string &file_name) : file_name(file_name) {}
  void initialise(string FN = "") {
    if (FN != "")
      file_name = FN;
    file.open(file_name,std::ios::out);
    file.close();
  }
  int write(T &t) { /* your code here */
    file.open(file_name, std::ios::app);
    int wh = file.tellp();
    file.seekp(wh);assert(wh!=-1);
    //std::cout<<wh<<" write\n";
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
    file.close();
    return wh;
  }
  // 用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void update(T &t, const int index) { /* your code here */
    file.open(file_name, std::ios::in | std::ios::out);
    file.seekp(index);
    assert(index!=-1);
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
    file.close();
  }

  // 读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void read(T &t, const int index) { /* your code here */
    file.open(file_name, std::ios::in | std::ios::out);
    file.seekg(index);assert(index!=-1);
    file.read(reinterpret_cast<char *>(&t), sizeof(T));
    file.close();
  }
};
const int mxn = 200;
struct info {
  char str[66];
  ll val = 0;
  info(ll va = 0) {val = va; memset(str,'\0',sizeof(str));}
  bool operator<(const info &b) const {
    int pd = strcmp(str, b.str);
    return (pd < 0) || (pd == 0 && val < b.val);
  }
  bool operator==(const info &b) const {
    return strcmp(str, b.str) == 0 && val == b.val;
  }
};
struct block_unit {
  int nxt = 0;
  int pos = 0;
  info first;
};
struct __block {
  // adjacent blocksize sums greater than 2*mxn
  int pool[500];
  block_unit message[500];
  int id = 0, head = 0;
  int getid() { return pool[0]? pool[pool[0]--]:++id; }
  void getback(int nw) {pool[++pool[0]] = nw;}
public:
  // remove id1 (tp = 1) or id1.nxt (tp = 0)
  void remove(int id1, bool tp = 0) {
    //std::cout<<_<<" "<<id1<<" "<<tp<<std::endl;
    if (tp) {
      if(message[id1].nxt) {
        head = message[id1].nxt;
        getback(id1);
      }
    } else {
      int id2 = message[id1].nxt, id3 = message[id2].nxt;
      message[id1].nxt = id3;
      getback(id2);
    }
  }
  int addnew(int id1) {
    int id2 = message[id1].nxt, id3 = getid();
    message[id3].nxt = id2;
    message[id1].nxt = id3;
    return id3;
  }
};
struct __node {
  info x[2 * mxn + 5]={};
  int size = 0;
};
namespace Map {
MemoryRiver<__block> Block("block");
MemoryRiver<__node> map("map");
__block block;
__node node;
void init() {
  if(access("block",F_OK)==0) {
    return;
  } else 
  {
    map.initialise();
    Block.initialise();
    string u = "";
    block.head = block.getid();
    block.message[block.head].first = info(0);
    strcpy(block.message[block.head].first.str , u.c_str());
    block.message[block.head].pos = map.write(node);
    Block.write(block);
  }
}
void read() { Block.read(block, 0); }
info getinfo(const char *str, ll value) {
  info temp;
  strcpy(temp.str, str), temp.val = value;
  return temp;
}
void insert(pair<int, int>, info);
void remove(info);
void find(const char *);
pair<int, int> getpos(info);
}; // namespace Map
pair<int, int> Map::getpos(info a) {
  int id1 = block.head, id2 = 0,id3 = 1;
  do {
    id2 = id1;
    id1 = block.message[id1].nxt;
  } while (id1 && block.message[id1].first < a);
  if (id1 && block.message[id1].first == a)
    return std::make_pair(id1, 0);
  id1 = id2;
  map.read(node, block.message[id1].pos);
  // if(a.val == 8){
  //   std::cout<<"block"<<" "<<block.message[id1].first.str<<" "<< block.message[id1].first.val<<std::endl;
  //   std::cout<<"getpos"<<" "<<id1<<std::endl;
  //   for(int i = 0;i < node.size; ++i)
  //     cout<<node.x[i].val<< std::endl;
  // }
  for (int i = 0; i < node.size; ++i) {
    if (node.x[i] == a)
      return std::make_pair(id1, i);
    if (a < node.x[i])
      return std::make_pair(id1, i);
  }
  return std::make_pair(id1, node.size);
}
void Map::find(const char *a) {
  // find a pos;
  int blockl = block.head, blockr = block.head;
  do{
    blockr = blockl; 
    blockl = block.message[blockl].nxt;
  }while(blockl && strcmp(block.message[blockl].first.str,a)<0);
  blockl = blockr;
  bool pd = 0;
  map.read(node,block.message[blockl].pos);
  for(int i = 0;i < node.size; ++i)
    if(!strcmp(node.x[i].str,a)) {
      cout<<node.x[i].val<<" ";
      pd = 1;
    }
  blockr = block.message[blockr].nxt;
  while(blockr && strcmp(block.message[blockr].first.str,a)==0){
    map.read(node, block.message[blockr].pos);
    for(int i = 0;i < node.size; ++i)
      if(!strcmp(node.x[i].str,a)) {
        cout<<node.x[i].val<<" ";
        pd = 1;
      }
    blockr = block.message[blockr].nxt;
  }
  cout<<(pd?"\n":"null\n");
}
#define st first
#define nd second
info c;
void Map::insert(pair<int, int> pos, info a) {
  bool blockupd = (pos.nd == 0);
  // uninitialised
  if(pos.nd == 0) block.message[pos.st].first = a;
  if (pos.nd == node.size) {
    node.x[node.size++] = a;
  } else {
    for(int i = node.size; i > pos.nd; --i)
      node.x[i] = node.x[i-1];
    node.x[pos.nd] = a;
    ++node.size;
  }
  
  if (node.size > (mxn * 2)) {
    __node nw;
    for (int i = mxn; i < node.size; ++i)
      nw.x[nw.size++] = node.x[i];
    node.size = mxn;
    int newid = block.addnew(pos.st);
    block.message[newid].first = nw.x[0];
    block.message[newid].pos = map.write(nw);
  } 
  map.update(node, block.message[pos.st].pos);
  Block.update(block, 0);
}
int cnt = 0;
void Map::remove(info a) {
  // node 
  pair<int,int> pos;
  int id1 = block.head, id2 = 0,pre = 0;
  do {
    pre = id2, id2 = id1;
    id1 = block.message[id1].nxt;
  } while (id1 && block.message[id1].first < a);
  // pre id2 id1
// 保证 ID1 在的块就是 first >= a
  pos = std::make_pair(id1, node.size);
//id1 恰好是
  if (id1 && block.message[id1].first == a){
    map.read(node, block.message[id1].pos);
    pos = std::make_pair(id1, 0);
    pre = id2;
    ++cnt;
  }
  // default pos.nd = node.size
  else {
    //否则 若 id2 == head?
    id1 = id2;
    map.read(node, block.message[id1].pos);
    for (int i = 0; i < node.size; ++i) {
      if (node.x[i] == a) {
        pos = std::make_pair(id1, i);
        ++cnt;
        break;
      } else 
      if (a < node.x[i]){
        //assert(0);
        break;
      }
    }
  }
  // if(_<=150){
  //   std::cout<<_<<" "<<node.size<<" "<<a.val<<std::endl;
  //   for(int i = 0; i < node.size; ++i) cout<<node.x[i].val<<" ";
  //   cout<<std::endl;
  // }
  if (pos.nd >= node.size || !(node.x[pos.nd] == a))
    return;
  // pos.st pos.nd 找到
  if(node.size == 1) {
    //size = 0
    --node.size;
    if(pre)block.remove(pre);
    else block.remove(pos.st,1);
  } else {
    for(int i = pos.nd; i < node.size - 1; ++i)
      node.x[i] = node.x[i+1];
    --node.size;
    if(!pos.nd) block.message[pos.st].first = node.x[0];
  }
    Block.update(block,0);
    map.update(node,block.message[pos.st].pos);
  // now we have block[pos.first] and node[pos.first]
  // remove node[pos.first].x[pos.second]
}
using namespace Map;
int n;
void printall() {
  return;
  int nw = block.head;
  do {
    map.read(node, block.message[nw].pos);
    std::cout<<node.size<<" Size: ";
    for(int i = 0; i < node.size; ++i)
      cout<<node.x[i].val<<" ";
    std::cout<<std::endl;
    nw = block.message[nw].nxt;
  }while(nw);
}
int main() {
  //freopen("input.txt","r",stdin);
  cin >> n;
  //clock_t start,end;
  //start = clock();  //开始时间
  string op, s;
  init();
  for (_ = 1; _ <= n; ++_) {
    cin >> op >> s;
    read();
    //if(_>=102&&_<=106)printall();
    if (op == "insert") {
      ll val;
      cin >> val;
      info b=getinfo(s.c_str(), val);
      insert(getpos(b), b);
    } else if (op == "delete") {
      ll val;
      cin >> val;
      info b;
      remove(getinfo(s.c_str(), val));
    } else if(op == "find"){
      find(s.c_str());
    }
  }
  //end = clock();   //结束时间
  //std::cerr<<"time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<std::endl;
  //std::cout<<cnt<<std::endl;
  return 0;
}
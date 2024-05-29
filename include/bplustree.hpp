#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include "pair.hpp"
#include "string.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

// namespace sjtu

#include <asm-generic/errno.h>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <string>
#include <utility>

// implement a b-plus-tree

namespace Bptree {

#define Bustub_PAGE_SIZE 8192
#define INTERNAL_PAGE_HEADER_SIZE 20
#define LEAF_PAGE_HEADER_SIZE 24

#define INDEX_TEMPLATE_ARGUMENTS                                               \
  template <typename KeyType, typename ValueType, typename GetType, int bpmSize = 100>

// define page type enum
enum class IndexPageType { INVALID_INDEX_PAGE = 0, LEAF_PAGE, INTERNAL_PAGE };
using page_id_t = int32_t;
using frame_id_t = int32_t;
struct RecycleList {
  // give next page_id
  page_id_t end_num_;
  std::fstream Recycle_filestream;
  std::string recycle_file_name_;

  void init(std::string file_name) {
    recycle_file_name_ = file_name + "_Recycle";
    Recycle_filestream.open(recycle_file_name_,
                            std::ios::binary | std::ios::in | std::ios::out);
    if (!Recycle_filestream.good()) {
      Recycle_filestream.clear();
      Recycle_filestream.open(recycle_file_name_,
                              std::ios::binary | std::ios::trunc |
                                  std::ios::out | std::ios::in);
      end_num_ = 0;
    } else {
      Recycle_filestream.read(reinterpret_cast<char *>(&end_num_),
                              sizeof(page_id_t));
    }
  }
  ~RecycleList() {
    if(end_num_ != -1) {
      Recycle_filestream.seekp(0);
      Recycle_filestream.write(reinterpret_cast<const char *>(&end_num_),
                              sizeof(page_id_t));
      Recycle_filestream.close();
    }
  }
  page_id_t GetVacancy() { 
    return ++end_num_; 
  }
  void recycle(page_id_t pg) {}
};
class DiskManager {
public:
#define OFFSET 4
  DiskManager() = default;
  void init(std::string &db_file) {
    this->file_name_ = db_file;
    db_io_.open(file_name_, std::ios::binary | std::ios::in | std::ios::out);
    if (!db_io_.good()) {
      db_io_.clear();
      db_io_.open(file_name_, std::ios::binary | std::ios::trunc |
                                  std::ios::out | std::ios::in);
    }
    dustbin.init(db_file);
  }
  void remove() {
    db_io_.close();
    std::remove(file_name_.c_str());
    dustbin.end_num_ = -1;
    dustbin.Recycle_filestream.close();
    std::remove(dustbin.recycle_file_name_.c_str());
    // std::cerr << "remove well" << std::endl;
  }
  ~DiskManager() { db_io_.close(); }
  /**
   * Write a page to the database file.
   * @param page_id id of the page
   * @param page_data raw page data
   */
  void addpage(page_id_t page_id) {
    char tmp[Bustub_PAGE_SIZE];
    db_io_.seekp((page_id - 1) * Bustub_PAGE_SIZE);
    db_io_.write(reinterpret_cast<const char *>(&tmp), Bustub_PAGE_SIZE);
  }
  void WritePage(page_id_t page_id, const char *page_data,
                 const char *type_data, int def_size) {
    db_io_.seekp((page_id - 1) * Bustub_PAGE_SIZE);
    db_io_.write(type_data, OFFSET);
    db_io_.write(page_data, def_size);
  }
  void ReadType(page_id_t page_id, char *type_data, int def_size = 4) {
    db_io_.seekg((page_id - 1) * Bustub_PAGE_SIZE);
    db_io_.read(type_data, sizeof(IndexPageType));
  }
  void ReadPage(page_id_t page_id, char *page_data, int def_size) {
    db_io_.seekg((page_id - 1) * Bustub_PAGE_SIZE + OFFSET);
    db_io_.read(page_data, def_size);
  }
  size_t GetFileSize() {
    db_io_.seekg(0, std::ios::end);
    return db_io_.tellg();
  }
  RecycleList dustbin;

protected:
  std::fstream db_io_;
  std::string file_name_;
};
class BPlusTreePage {
private:
  // Member variables, attributes that both internal and leaf page share
  IndexPageType page_type_;
  int size_;
  int max_size_;

public:
  BPlusTreePage() = default;
  BPlusTreePage(const BPlusTreePage &other) = default;
  ~BPlusTreePage() = default;
  auto IsLeafPage() const -> bool {
    return page_type_ == IndexPageType::LEAF_PAGE;
  }
  void SetPageType(IndexPageType page_type) { page_type_ = page_type; }
  auto GetSize() const -> int { return size_; }
  void SetSize(int size) { size_ = size; }
  void IncreaseSize(int amount) { size_ += amount; }

  auto GetMaxSize() const -> int { return max_size_; }
  void SetMaxSize(int max_size) { max_size_ = max_size; }
  auto GetMinSize() { return max_size_ / 2; }
};

INDEX_TEMPLATE_ARGUMENTS
class BPlusTree {
#define INTERNAL_PAGE_SIZE                                                     \
  ((Bustub_PAGE_SIZE - INTERNAL_PAGE_HEADER_SIZE) /                            \
   (sizeof(InternalMappingType)))
#define B_PLUS_TREE_LEAF_PAGE_TYPE                                             \
  BPlusTreeLeafPage<KeyType, ValueType, KeyComparator>
#define LEAF_PAGE_SIZE                                                         \
  ((Bustub_PAGE_SIZE - LEAF_PAGE_HEADER_SIZE) / sizeof(MappingType))
  // *作为指针只在内存，在外存读写中不需要用到。
#define MappingType pair<KeyType, ValueType>
#define InternalMappingType pair<KeyType, page_id_t>
  struct BPlusTreeInternalPage : public BPlusTreePage {
  public:
    // Delete all constructor / destructor to ensure memory safety
    /**
     * Writes the necessary header information to a newly created page, must be
     * called after the creation of a new page to make a valid
     * `BPlusTreeInternalPage`
     * @param max_size Maximal size of the page
     */
    BPlusTreeInternalPage() {
      SetMaxSize(INTERNAL_PAGE_SIZE);
      SetPageType(IndexPageType::INTERNAL_PAGE);
      SetSize(0);
    }

    /**
     * @param index The index of the key to get. Index must be non-zero.
     * @return Key at index
     */

    auto KeyAt(int index)  -> KeyType { return array_[index].first; }
    /*
      return a pair first:pos, second: page_id_t
    */
    pair<size_t, page_id_t> find(const KeyType &key) {
      size_t pos = 0, sz = GetSize();
      for (int length = (1 << int(log2(sz))); length; length >>= 1)
        if (pos + length < sz && !(key < KeyAt(pos + length)))
          pos += length;
      return pair<size_t, page_id_t>(pos, array_[pos].second);
    }
    /*  array_[index] = pair{key, val}
        顺次移动
    */
    void ModKeyAt(const KeyType &key, int index) { array_[index].first = key; }
    void SetKeyAt(int index, const KeyType &key, const page_id_t &val) {
      for (int i = GetSize(); i > index; --i)
        array_[i] = std::move(array_[i - 1]);
      IncreaseSize(1);
      array_[index] = {key, val};
    }
    void DelAt(int index) {
      for (int i = index; i + 1 < GetSize(); ++i)
        array_[i] = std::move(array_[i + 1]);
      IncreaseSize(-1);
    }
    /**
     * @param index The index to search for
     * @return The value at the index
     */
    /*
      指向第index个的子结点
    */
    auto ValAt(int index) const -> page_id_t { return array_[index].second; }
    auto ToString() const -> std::string {
      std::string kstr = "(";
      bool first = true;

      for (int i = 0; i < GetSize(); i++) {
        KeyType key = KeyAt(i);
        if (first) {
          first = false;
        } else {
          kstr.append(",");
        }
        kstr.append("( " + std::to_string(key.first) + " , " +
                    std::to_string(key.second) + ")");
      }
      kstr.append(")");

      return kstr;
    }

  public:
    // Flexible array member for page data.
    InternalMappingType array_[INTERNAL_PAGE_SIZE];
  };
  struct BPlusTreeLeafPage : public BPlusTreePage {
  public:
    // Delete all constructor / destructor to ensure memory safety
    /**
     * After creating a new leaf page from buffer pool, must call initialize
     * method to set default values
     * @param max_size Max size of the leaf node
     */
    BPlusTreeLeafPage() {
      SetMaxSize(LEAF_PAGE_SIZE);
      SetPageType(IndexPageType::LEAF_PAGE);
      SetSize(0);
    }
    // Helper methods
    auto GetNextPageId() const -> page_id_t { return next_page_id_; }
    void SetNextPageId(page_id_t next_page_id) { next_page_id_ = next_page_id; }
    auto KeyAt(int index) const -> KeyType { return array_[index].first; }
    auto ValAt(int index) const -> ValueType { return array_[index].second; }
    size_t find(const KeyType &key, const ValueType &value) {
      size_t pos = 0, sz = GetSize();
      for (int length = (1 << int(log2(sz))); length; length >>= 1)
        if (pos + length < sz && !(key < KeyAt(pos + length)))
          pos += length;
      if (KeyAt(pos) < key)
        return (pos + 1) << 1;
      return (pos << 1) + (array_[pos].first == key);
    }
    void SetKeyAt(int index, const KeyType &key, const ValueType val) {
      for (int i = GetSize(); i > index; --i)
        array_[i] = std::move(array_[i - 1]);
      IncreaseSize(1);
      array_[index] = {key, val};
    }
    void DelAt(int index) {
      for (int i = index; i + 1 < GetSize(); ++i)
        array_[i] = std::move(array_[i + 1]);
      IncreaseSize(-1);
    }
    void ModKeyAt(const KeyType &key, int index) { array_[index].first = key; }
    /**
     * @brief For test only return a string representing all keys in
     * this leaf page formatted as "(key1,key2,key3,...)"
     *
     * @return The string representation of all keys in the current internal
     * page
     */
    auto ToString() const -> std::string {
      std::string kstr = "(";
      bool first = true;

      for (int i = 0; i < GetSize(); i++) {
        KeyType key = KeyAt(i);
        if (first) {
          first = false;
        } else {
          kstr.append(",");
        }
        kstr.append("( " + std::to_string(key.first) + " , " +
                    std::to_string(key.second) + ")");
      }
      kstr.append(")");
      return kstr;
    }
  public:
    page_id_t next_page_id_;
    // Flexible array member for page data.
    MappingType array_[LEAF_PAGE_SIZE];
  };
  struct PtrHead {
    int pos, count, pin;
    PtrHead(int a, int b, int c) : pos(a), count(b), pin(c) {}
  };
  struct Ptr;
  template<int MaxSize>
  struct BufferPoolManager {
    DiskManager disk;
    size_t sz = 0;
    int pos[MaxSize], *rt;
    Ptr tmp[MaxSize];
    std::fstream root;
    std::string root_name;
    void init(std::string file_name, int *header_id) {
      memset(pos, 0, sizeof(pos));
      this->disk.init(file_name);
      rt = header_id;
      root_name = file_name + "_root";
      root.open(root_name, std::ios::binary | std::ios::in | std::ios::out);
      if (!root.good()) {
        root.clear();
        root.open(root_name, std::ios::binary | std::ios::trunc |
                                 std::ios::out | std::ios::in);
        *header_id = -1;
      } else {
        root.read(reinterpret_cast<char *>(header_id), sizeof(int));
      }
    }
    void remove() {
      root.close();
      std::remove(root_name.c_str());
      disk.remove();
    }
    void getpos(PtrHead *head) { head->pos = disk.dustbin.GetVacancy(); }
    void setpos(PtrHead *head, int pos_) { head->pos = pos_; }

    void WritePage(PtrHead *head, BPlusTreePage *content) {
      int sz;
      IndexPageType ind = content->IsLeafPage() ? IndexPageType::LEAF_PAGE
                                                : IndexPageType::INTERNAL_PAGE;
      if (ind == IndexPageType::LEAF_PAGE)
        sz = sizeof(BPlusTreeLeafPage);
      else
        sz = sizeof(BPlusTreeInternalPage);
      disk.WritePage(head->pos, reinterpret_cast<const char *>(content),
                     reinterpret_cast<const char *>(&ind), sz);
      head->pin = 0;
    }
    void ReadPage(page_id_t id, BPlusTreePage *&content) {
      IndexPageType type;
      disk.ReadType(id, reinterpret_cast<char *>(&type));
      size_t sz;
      if (type == IndexPageType::LEAF_PAGE)
        content = new BPlusTreeLeafPage, sz = sizeof(BPlusTreeLeafPage);
      else
        content = new BPlusTreeInternalPage, sz = sizeof(BPlusTreeInternalPage);
      disk.ReadPage(id, reinterpret_cast<char *>(content), sz);
    }
    void add_content(int pos_, page_id_t pg, Ptr *pointer) {
      // std::cout << "add " << pos_ << " " << pg << " " << pointer->content
      //           << std::endl;
      // assert(pointer->content != nullptr);
      if (pointer->content == nullptr)
        ReadPage(pg, pointer->content);
      tmp[pos_] = *pointer, pos[pos_] = pg;
    }
    // 区分在末尾析构，还是中途析构
    void del_content(int pos_, int tp = 0) {
      // assert(tmp[pos_].head != nullptr);
      if (tmp[pos_].head->pin)
        WritePage(tmp[pos_].head, tmp[pos_].content);
      // bug!!!
      if (tp)
        delete tmp[pos_].head, tmp[pos_].head = nullptr;
      pos[pos_] = 0;
      if (tmp[pos_].content->IsLeafPage())
        delete static_cast<BPlusTreeLeafPage *>(tmp[pos_].content);
      else
        delete static_cast<BPlusTreeInternalPage *>(tmp[pos_].content);
      // std::cerr << 233 << std::endl;
    }

    void get_content(page_id_t pos_, Ptr *pointer) {
      if (pos_ == -1)
        goto notfound;
      for (int i = 0; i < sz; ++i)
        if (pos_ == pos[i]) {
          *pointer = tmp[i];
          return;
        }
    notfound:;
      if (pointer->head == nullptr)
        pointer->head = new PtrHead(pos_, 0, 0);
      if (pos_ == -1) {
        getpos(pointer->head);
        pos_ = pointer->head->pos;
        disk.addpage(pos_);
      } else {
        // std::cerr << " Rea Pa " << pos_ << std::endl;
        // assert(0);
        ReadPage(pointer->head->pos, pointer->content);
      }
      ++pointer->head->count;
      // make sure pointer is valid
      if (sz == MaxSize) {
        for (int i = sz - 1; i >= 0; --i) {
          if (tmp[i].head->count == 1) {
            del_content(i, 1);
            add_content(i, pos_, pointer);
            return;
          }
        }
      } else {
        add_content(sz++, pos_, pointer);
      }
    }
    void exit() {
      
      for (size_t i = 0; i < sz; ++i) {
        if (pos[i])
          del_content(i);
      }
      root.seekp(0);
      root.write(reinterpret_cast<const char *>(rt), sizeof(int));
      root.close();
    }
    /*
      if bufferpool is full
        delete reference = full type
    */
  };
  struct Ptr {
    BPlusTreePage *content = nullptr;
    PtrHead *head = nullptr;
    BufferPoolManager<bpmSize> *bpm = nullptr;
    Ptr() = default;
    void getpos() { bpm->getpos(head); }
    Ptr(BufferPoolManager<bpmSize> *bpm_, BPlusTreePage *cont = nullptr, int pos_ = -1) {
      if (pos_) {
        bpm = bpm_;
        content = cont;
        bpm->get_content(pos_, this);
      }
    }
    Ptr(const Ptr &rhs) {
      content = rhs.content;
      head = rhs.head;
      bpm = rhs.bpm;
      ++head->count;
    }
    void delPtr() {
      if (head)
        --head->count;
    }
    Ptr &operator=(const Ptr &rhs) {
      if (content && content == rhs.content) {
        ++head->count;
        return *this;
      }
      if (this == &rhs)
        return *this;
      if (content)
        delPtr();
      content = rhs.content;
      head = rhs.head;
      if (head)
        ++head->count;
      bpm = rhs.bpm;
      return *this;
    }

    void Dirty() { head->pin = 1; }
    ~Ptr() { delPtr(); }
    BPlusTreePage *operator->() { return content; }
  };

public:
  void printtree() {
    // Ptr(&bpm, nullptr, header_page_id_);
    // return;
    if (header_page_id_ != -1)
      printtree(Ptr(&bpm, nullptr, header_page_id_));
  }
  void printtree(Ptr rt, int dep = 0) {
    for (int i = 0; i < dep; i++)
      std::cout << "  ";
    std::cout << rt.head->pos << " " << to_string(rt.content);
    if (rt.content->IsLeafPage()) {
      std::cout << static_cast<BPlusTreeLeafPage *>(rt.content)->GetNextPageId()
                << std::endl;
      return;
    } else {
      std::cout << " " << std::endl;
    }
    for (int i = 0; i < rt.content->GetSize(); ++i)
      printtree(
          Ptr(rt.bpm, nullptr,
              static_cast<BPlusTreeInternalPage *>(rt.content)->ValueAt(i)),
          dep + 1);
  }
  BPlusTree() = default;
  BPlusTree(std::string disk_name)
      : leaf_max_size_(LEAF_PAGE_SIZE), internal_max_size_(INTERNAL_PAGE_SIZE) {
    // page_id manager?
    // std::cerr << "size" << leaf_max_size_ << " " << std::endl;
    bpm.init(disk_name, &header_page_id_);
  }
  void exit(bool tp = 0) {
    if (tp) {
      return remove();
    }
    if (~header_page_id_)
      bpm.exit();
  }
  void remove() {
    header_page_id_ = -1;
    bpm.remove();
  }

  void Init(std::string disk_name) {
    leaf_max_size_ = LEAF_PAGE_SIZE, internal_max_size_ = INTERNAL_PAGE_SIZE;
    bpm.init(disk_name, &header_page_id_);
  }
  // find the position to insert key, if such position do not exist return
  Ptr FindPos(const KeyType &key) {
    if (header_page_id_ == -1)
      return Ptr();
    Ptr rt(&bpm, nullptr, header_page_id_);
    while (rt.content && !rt->IsLeafPage()) {
      auto res = (static_cast<BPlusTreeInternalPage *>(rt.content)->find(key));
      rt = Ptr(&bpm, nullptr, res.second);
    }
    return rt;
  }
  // Ptr(nullptr) (last Key <= key)
  std::string to_string(BPlusTreePage *node) {
    if (node->IsLeafPage())
      return static_cast<BPlusTreeLeafPage *>(node)->ToString();
    else
      return static_cast<BPlusTreeInternalPage *>(node)->ToString();
  }
  void FindPosVector(const KeyType &key, sjtu::vector<Ptr> *v,
                     sjtu::vector<int> *v1) {
    if (header_page_id_ == -1)
      return;
    Ptr rt(&bpm, nullptr, header_page_id_);
    // std::cerr << "rt " << to_string(rt.content) << std::endl;
    v->push_back(rt);
    // std::cerr << "route for " << key.second << " " << rt.content << " " <<
    // to_string(rt.content) << " |";
    while (rt.content && !rt->IsLeafPage()) {
      auto res = (static_cast<BPlusTreeInternalPage *>(rt.content)->find(key));
      // std::cerr << res.first << "] = " << to_string(rt.content) << " |";
      rt = Ptr(&bpm, nullptr, res.second);
      // std::cerr << "key = " << res.first << " " << res.second << " " <<
      // to_string(rt.content) << std::endl;
      v->push_back(rt);
      v1->push_back(res.first);
    }
    // std::cerr << std::endl;
  }
  BPlusTreeLeafPage *leaf(BPlusTreePage *page) {
    return static_cast<BPlusTreeLeafPage *>(page);
  }
  BPlusTreeInternalPage *intern(BPlusTreePage *page) {
    return static_cast<BPlusTreeInternalPage *>(page);
  }
  KeyType KeyAt(BPlusTreePage* page, const int& index) {
    if(page->IsLeafPage())
      return leaf(page)->KeyAt(index);
    else 
      return intern(page)->KeyAt(index);
  }
  void DelAt(BPlusTreePage* page, const int& index) {
    if(page->IsLeafPage())
      return leaf(page)->DelAt(index);
    else
      return intern(page)->DelAt(index);
  }
  bool Insert(const KeyType &key, const ValueType &value) {
    sjtu::vector<Ptr> v;
    sjtu::vector<int> v1;
    FindPosVector(key, &v, &v1);
    if (v.empty()) {
      BPlusTreeLeafPage *new_page = new BPlusTreeLeafPage;
      new_page->SetKeyAt(0, key, value);
      new_page->SetNextPageId(0);
      Ptr pos(&bpm, new_page);
      pos.Dirty();
      header_page_id_ = pos.head->pos;
      return 1;
    }
    Ptr pos(v.back());
    BPlusTreeLeafPage *cont = static_cast<BPlusTreeLeafPage *>(pos.content);
    size_t index = cont->find(key, value);
    if (index & 1)
      return 0;
    index >>= 1;
    if (cont->GetSize() < leaf_max_size_) {
      cont->SetKeyAt(index, key, value);
      pos.Dirty();
      return 1;
    }
    BPlusTreeLeafPage *new_page = new BPlusTreeLeafPage;
    for (int i = (leaf_max_size_ >> 1); i < leaf_max_size_; ++i)
      new_page->SetKeyAt(i - (leaf_max_size_ >> 1), cont->KeyAt(i),
                         cont->ValAt(i));
    if (index < (leaf_max_size_ >> 1)) {
      cont->SetSize((leaf_max_size_ >> 1));
      cont->SetKeyAt(index, key, value);
    } else {
      new_page->SetKeyAt(index - (leaf_max_size_ >> 1), key, value);
      cont->SetSize((leaf_max_size_ >> 1));
    }
    // upload the new split page
    Ptr new_pos(&bpm, new_page);

    new_page->SetNextPageId(cont->GetNextPageId());
    cont->SetNextPageId(new_pos.head->pos);
    new_pos.Dirty(), pos.Dirty();

    // check if their father is full
    BPlusTreePage *new1_page = new_page;
    v.pop_back();
    if (v.empty()) {
      BPlusTreeInternalPage *new_root = new BPlusTreeInternalPage;
      // std::cerr << "new root" << pos.content->GetSize() << " " <<
      // new_pos.content->GetSize();
      new_root->SetKeyAt(0, KeyAt(pos.content, 0), pos.head->pos);
      new_root->SetKeyAt(1, KeyAt(new_pos.content, 0), new_pos.head->pos);
      Ptr rt(&bpm, new_root);
      rt.Dirty();
      header_page_id_ = rt.head->pos;
      return 1;
    }
    while (!v.empty()) {
      Ptr fa = v.back();
      BPlusTreeInternalPage *fa_cont =
          static_cast<BPlusTreeInternalPage *>(fa.content);
      index = v1.back() + 1; // when new_page insert
      /*
        fa[index] = new1_page
      */
      if (fa_cont->GetSize() < internal_max_size_) {
        fa_cont->SetKeyAt(index, KeyAt(new1_page, 0), new_pos.head->pos);
        fa.Dirty();
        return 1;
      }

      BPlusTreeInternalPage *new_fa = new BPlusTreeInternalPage;
      for (int i = internal_max_size_ >> 1; i < internal_max_size_; ++i)
        new_fa->SetKeyAt(i - (internal_max_size_ >> 1), fa_cont->KeyAt(i),
                         fa_cont->ValAt(i));
      fa->SetSize(internal_max_size_ >> 1);
      if (index < (internal_max_size_ >> 1)) {
        fa_cont->SetKeyAt(index, KeyAt(new1_page, 0), new_pos.head->pos);
      } else {
        new_fa->SetKeyAt(index - (internal_max_size_ >> 1), KeyAt(new1_page, 0),
                         new_pos.head->pos);
      }
      Ptr new_fa_page(&bpm, new_fa);
      fa.Dirty(), new_fa_page.Dirty();
      new_pos = new_fa_page, new1_page = new_fa;
      v.pop_back(), v1.pop_back();
      // last case: root needs to split
      if (v.empty()) {
        BPlusTreeInternalPage *new_root = new BPlusTreeInternalPage;
        new_root->SetKeyAt(0, KeyAt(fa_cont, 0), fa.head->pos);
        new_root->SetKeyAt(1, KeyAt(new_fa, 0), new_fa_page.head->pos);
        Ptr rt(&bpm, new_root);
        rt.Dirty();
        header_page_id_ = rt.head->pos;
        v.clear(), v1.clear();
        return 1;
      }
    }

    return 1;
  }
  void update(Ptr &fa, BPlusTreePage *fa_cont, Ptr &pre,
              BPlusTreePage *pre_cont, Ptr &nxt, BPlusTreePage *nxt_cont,
              size_t index, int dx) {
    if (pre_cont->IsLeafPage()) {
      if (dx > 0) {
        // bro = pos_cont + 1 bro.first = pos_cont.last
        auto res = MappingType{leaf(nxt_cont)->KeyAt(0), leaf(nxt_cont)->ValAt(0) };
        leaf(pre_cont)->SetKeyAt(pre_cont->GetSize(), res.first, res.second);
        leaf(nxt_cont)->DelAt(0);
      } else {
        // pos_cont = bro + 1
        auto res = MappingType{leaf(pre_cont)->KeyAt(pre_cont->GetSize() - 1),
                               leaf(pre_cont)->ValAt(pre_cont->GetSize() - 1)};
        leaf(nxt_cont)->SetKeyAt(0, res.first, res.second);
        leaf(pre_cont)->DelAt(pre_cont->GetSize() - 1);
      }
      static_cast<BPlusTreeInternalPage *>(fa_cont)->ModKeyAt
          (leaf(nxt_cont)->KeyAt(0), index + 1);
      fa.Dirty(), pre.Dirty(), nxt.Dirty();
    } else {
      if (dx > 0) {
        // bro = pos_cont + 1 bro.first = pos_cont.last
        auto res = InternalMappingType{intern(nxt_cont)->KeyAt(0), intern(nxt_cont)->ValAt(0) };
        intern(pre_cont)->SetKeyAt(pre_cont->GetSize(), res.first, res.second);
        intern(nxt_cont)->DelAt(0);
      } else {
        // pos_cont = bro + 1
        auto res = InternalMappingType{intern(pre_cont)->KeyAt(pre_cont->GetSize() - 1),
                               intern(pre_cont)->ValAt(pre_cont->GetSize() - 1)};
        intern(nxt_cont)->SetKeyAt(0, res.first, res.second);
        intern(pre_cont)->DelAt(pre_cont->GetSize() - 1);
      }
      static_cast<BPlusTreeInternalPage *>(fa_cont)->ModKeyAt
          (intern(nxt_cont)->KeyAt(0), index + 1);
      fa.Dirty(), pre.Dirty(), nxt.Dirty();
  }
}

  void merge(Ptr &fa, BPlusTreePage *fa_cont, Ptr &pre, BPlusTreePage *pre_cont,
             Ptr &nxt, BPlusTreePage *nxt_cont, size_t index) {
    size_t offset = pre_cont->GetSize();
    if(nxt_cont->IsLeafPage()) {
      for (int i = 0; i < nxt_cont->GetSize(); ++i)
        leaf(pre_cont)->SetKeyAt(offset + i, KeyAt(nxt_cont, i), leaf(nxt_cont)->ValAt(i));
    } else {
      for (int i = 0; i < nxt_cont->GetSize(); ++i)
        intern(nxt_cont)->SetKeyAt(offset + i, KeyAt(nxt_cont, i), intern(nxt_cont)->ValAt(i));  
    }

    if (pre_cont->IsLeafPage())
      static_cast<BPlusTreeLeafPage *>(pre_cont)->SetNextPageId(
          static_cast<BPlusTreeLeafPage *>(nxt_cont)->GetNextPageId());
    // printtree(Ptr(&bpm, nullptr, header_page_id_));
    pre.Dirty(), nxt.Dirty(), fa.Dirty();
    nxt_cont->SetSize(0);
  }
  // Remove a key and its value from this B+ tree.
  void Remove(const KeyType &key) {
    // if(key.second == 2)
    //   std::cerr << "让我看看" << std::endl;
    sjtu::vector<Ptr> v;
    sjtu::vector<int> v1;
    FindPosVector(key, &v, &v1);
    if (v.empty())
      return;
    Ptr pos = v.back();
    BPlusTreePage *cont = pos.content;
    size_t index =
        static_cast<BPlusTreeLeafPage *>(cont)->find(key, key.second);
    if (index % 2 == 0)
      return;
    index >>= 1;
    if (index == 0 && cont->GetSize() > 1) {
      for (size_t i = v1.size(); i; --i) {
        Ptr fa = v[i - 1];
        static_cast<BPlusTreeInternalPage *>(fa.content)
            ->ModKeyAt(KeyAt(cont, 1), v1[i - 1]);
        // std::cerr << (to_string(v[i - 1].content)) << " " << v1[i - 1] << " |
        // ";
        fa.Dirty();
        if (v1[i - 1] != 0)
          break;
      }
    }
    v.pop_back();
    // recursively: cont.size() < getminsize()
    while (v.size()) {
      // std::cerr << index << std::endl;
      DelAt(cont, index);
      pos.Dirty();
      if (cont->GetSize() >= cont->GetMinSize()) {
        return;
      }
      Ptr fa = v.back();
      BPlusTreeInternalPage *fa_cont =
          static_cast<BPlusTreeInternalPage *>(fa.content);
      int index1 = v1.back();
      // bro.index = fa.index + flag
      int flag;
      Ptr bro;
      if (index1 + 1 < fa_cont->GetSize())
        bro = Ptr(&bpm, nullptr, fa_cont->ValAt(index1 + 1)), flag = 1;
      else
        bro = Ptr(&bpm, nullptr, fa_cont->ValAt(index1 - 1)), flag = -1;
      BPlusTreeInternalPage *bro_cont =
          static_cast<BPlusTreeInternalPage *>(bro.content);
      if (bro_cont->GetSize() > bro_cont->GetMinSize()) {
        // swap
        if (flag > 0)
          update(fa, fa_cont, pos, cont, bro, bro_cont, index1, 1);
        else
          update(fa, fa_cont, bro, bro_cont, pos, cont, index1 - 1, -1);
        return;
      } else {
        if (flag > 0)
          merge(fa, fa_cont, pos, cont, bro, bro_cont, index1), ++index1;
        // REMOVE fa_cont.index1
        else
          merge(fa, fa_cont, bro, bro_cont, pos, cont, index1 - 1);
        v1.pop_back(), v.pop_back();
        pos = fa;
        cont = fa_cont;
        index = index1;
      }
    }
    DelAt(cont, index);
    pos.Dirty();
    if (cont->GetSize() == 1 && !pos.content->IsLeafPage() &&
        pos.head->pos == header_page_id_) {
      header_page_id_ = intern(pos.content)->ValAt(0);
      // std::cerr << "删掉父亲"  << std::endl;
      return;
    }
    if (cont->GetSize() == 0)
      header_page_id_ = -1;
  }
  // update key&value -> newkey&value
  void update(const KeyType &key, const KeyType &newkey) {
    // if(key.second == 2)
    //   std::cerr << "让我看看" << std::endl;
    sjtu::vector<Ptr> v;
    sjtu::vector<int> v1;
    FindPosVector(key, &v, &v1);
    if (v.empty())
      return;
    Ptr pos = v.back();
    BPlusTreePage *cont = pos.content;
    size_t index =
        static_cast<BPlusTreeLeafPage *>(cont)->find(key, key.second);
    if (index % 2 == 0)
      assert(0);
    index >>= 1;
    static_cast<BPlusTreeLeafPage *>(cont)->ModKeyAt(newkey, index);
    pos.Dirty();
  }
  // Return the value associated with a given key
  int find(const GetType &key) {
    sjtu::vector<int> result;
    GetValue(key, &result);
    return result.empty() ? 0 : result[0];
  }
  void GetUniqueValue(const GetType &key, ValueType &val) {
    Ptr l = FindPos(KeyType{key, val});
    BPlusTreeLeafPage *tmp;
    if (l.content)
      tmp = static_cast<BPlusTreeLeafPage *>(l.content);
    else
      return val.id = -1, void();
    for (int i = 0; i < tmp->GetSize(); ++i) {
      if (key < tmp->KeyAt(i).first)
        break;
      if (tmp->KeyAt(i).first == key && tmp->KeyAt(i).second.id == val.id) {
        val = tmp->ValAt(i);
        return;
      }
    }
    val.id = -1;
  }
  void GetValue(const GetType &key, sjtu::vector<ValueType> *result) {
    Ptr l = FindPos(KeyType{key, ValueType()});
    BPlusTreeLeafPage *tmp;
    int id = -1;
    while (
        l.content &&
        (tmp = static_cast<BPlusTreeLeafPage *>(l.content))->KeyAt(0).first <=
            key) {
      // assert(id);
      for (int i = 0; i < tmp->GetSize(); ++i)
        if (tmp->KeyAt(i).first == key)
          result->push_back(tmp->KeyAt(i).second);
      l = Ptr(&bpm, nullptr,
              id =
                  static_cast<BPlusTreeLeafPage *>(l.content)->GetNextPageId());
    }
  }
  // Return the page id of the root node
  page_id_t GetRootPageId() { return header_page_id_; }
  BufferPoolManager<bpmSize> bpm;
  void debug() {}

private:
  /* Debug Routines for FREE!! */
  // member variable
  bool isR = 0;
  sjtu::string<20> index_name_;
  int leaf_max_size_;
  int internal_max_size_;
  page_id_t header_page_id_ = -1;
};

} // namespace Bptree
// sjtu::vector<int> result;
// Bptree::BPlusTree<pair<unsigned long long, int>, int> Bpt("disk");
#endif
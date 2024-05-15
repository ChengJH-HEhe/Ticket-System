#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <string.h>

#include <cmath>

namespace sjtu {
template <typename T> class vector {
  size_t m_size, m_cap;
  T *data;
  std::allocator<T> a;

public:
  class const_iterator;
  class iterator {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

  private:
    pointer dat;
    pointer begin;

  public:
    iterator(pointer dat2 = nullptr, pointer begin2 = nullptr)
        : begin(begin2), dat(dat2){};
    iterator operator+(const int &n) const {
      // TODO
      auto tmp = dat;
      tmp += n;
      return iterator(tmp, begin);
    }
    iterator operator-(const int &n) const {
      // TODO
      auto tmp = dat;
      tmp -= n;
      return iterator(tmp, begin);
    }
    int operator-(const iterator &rhs) const {
      // TODO
      if (begin != rhs.begin)
        throw;
      return dat - rhs.dat;
    }
    iterator &operator+=(const int &n) {
      // TODO
      *this = *this + n;
      return *this;
    }
    iterator &operator-=(const int &n) {
      // TODO
      *this = *this - n;
      return *this;
    }
    /**
     * TODO iter++
     */
    iterator operator++(int) {
      auto tmp = this;
      ++this;
      return tmp;
    }
    /**
     * TODO ++iter
     */
    iterator &operator++() {
      // *this = *this+1;
      ++dat;
      return *this;
    }
    /**
     * TODO iter--
     */
    iterator operator--(int) {
      auto tmp = this;
      --this;
      return tmp;
    }
    /**
     * TODO --iter
     */
    iterator &operator--() {
      --dat;
      return *this;
    }
    /**
     * TODO *it
     */
    T &operator*() const { return *dat; }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory address).
     */
    bool operator==(const iterator &rhs) const { return dat == rhs.dat; }
    bool operator==(const const_iterator &rhs) const { return dat == rhs.dat; }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
  };
  /**
   * TODO
   * has same function as iterator, just for a const object.
   */
  class const_iterator {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

  private:
    /*TODO*/
    const T *dat;
    const T *begin;

  public:
    const_iterator(const T *dat2 = nullptr, const T *begin2 = nullptr)
        : begin(begin2), dat(dat2){};
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    const_iterator operator+(const int &n) const {
      // TODO
      auto tmp = this;
      tmp += n;
      return tmp;
    }
    const_iterator operator-(const int &n) const {
      // TODO
      auto tmp = this;
      tmp -= n;
      return tmp;
    }
    // return the distance between two const_iterators,
    // if these two const_iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const const_iterator &rhs) const {
      // TODO
      if (begin != rhs.begin)
        throw;
      return rhs.dat - dat;
    }
    const_iterator &operator+=(const int &n) {
      // TODO
      *this = *this + n;
      return *this;
    }
    const_iterator &operator-=(const int &n) {
      // TODO
      *this = *this - n;
      return *this;
    }
    /**
     * TODO iter++
     */
    const_iterator operator++(int) {
      auto tmp = this;
      ++this;
      return tmp;
    }
    /**
     * TODO ++iter
     */
    const_iterator &operator++() {
      // *this = *this+1;
      ++dat;
      return *this;
    }
    /**
     * TODO iter--
     */
    const_iterator operator--(int) {
      auto tmp = this;
      --this;
      return tmp;
    }
    /**
     * TODO --iter
     */
    const_iterator &operator--() {
      --dat;
      return *this;
    }
    /**
     * TODO *it
     */
    const T &operator*() const { return *dat; }
    /**
     * a operator to check whether two const_iterators are same (pointing to the
     * same memory address).
     */
    bool operator==(const iterator &rhs) const { return dat == rhs.dat; }
    bool operator==(const const_iterator &rhs) const { return dat == rhs.dat; }
    /**
     * some other operator for const_iterator.
     */
    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
  };
  /**
   * TODO Constructs
   * At least two: default constructor, copy constructor
   */
  vector() {
    m_size = 0, m_cap = 0;
    data = nullptr;
  }
  vector(const vector &other) {
    m_size = other.m_size, m_cap = other.m_cap;
    data = a.allocate(m_cap);
    for (int i = 0; i < m_size; ++i)
      std::construct_at(&data[i], other.data[i]);
  }
  /**
   * TODO Destructor
   */
  ~vector() {
    for (int i = 0; i < m_size; ++i)
      std::destroy_at(data + i);
    a.deallocate(data, m_cap);
    m_size = m_cap = 0;
  }
  /**
   * TODO Assignment operator
   */
  vector &operator=(const vector &other) {
    if (this == &other)
      return *this;
    if (data) {
      clear();
      a.deallocate(data, m_cap);
    }
    if (other.data == nullptr) {
      m_size = m_cap = 0;
      data = nullptr;
    } else {
      m_size = other.m_size, m_cap = other.m_cap;
      data = a.allocate(m_cap);
      for (int i = 0; i < m_size; ++i)
        std::construct_at(data + i, other.data[i]);
    }
    return *this;
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   */
  T &at(const size_t &pos) {
    if (pos >= m_size)
      throw;
    return *(data + pos);
  }
  const T &at(const size_t &pos) const {
    if (pos >= m_size)
      throw;
    return *(data + pos);
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   * !!! Pay attentions
   *   In STL this operator does not check the boundary but I want you to do.
   */
  T &operator[](const size_t &pos) { return at(pos); }
  const T &operator[](const size_t &pos) const { return at(pos); }
  /**
   * access the first element.
   * throw container_is_empty if size == 0
   */
  const T &front() const {
    if (empty())
      throw;
    return *data;
  }
  /**
   * access the last element.
   * throw container_is_empty if size == 0
   */
  const T &back() const {
    if (empty())
      throw;
    return *(data + m_size - 1);
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() { return iterator(data, data); }
  const_iterator cbegin() const { return const_iterator(data, data); }
  /**
   * returns an iterator to the end.
   */
  iterator end() { return iterator(data + m_size, data); }
  const_iterator cend() const { return const_iterator(data + m_size, data); }
  /**
   * checks whether the container is empty
   */
  bool empty() const { return !m_size; }
  /**
   * returns the number of elements
   */
  size_t size() const { return m_size; }
  /**
   * clears the contents
   */
  void clear() {
    for (int i = 0; i < m_size; ++i)
      std::destroy_at(data + i);
    m_size = 0;
  }
  /**
   * inserts value before pos
   * returns an iterator pointing to the inserted value.
   */
  void grow_capacity() {
    if ((m_size + 1) <= m_cap)
      return;
    size_t new_cap = (m_size + 1) * 2;
    // m_cap->new_cap
    T *old_data = data;
    data = a.allocate(new_cap);
    for (int i = 0; i < m_size; ++i)
      std::construct_at(data + i, std::move(old_data[i]));
    if (old_data)
      a.deallocate(old_data, m_cap);
    m_cap = new_cap;
  }
  void shrink_to_fit() {
    if (m_cap > m_size * 2)
      m_cap = 2 * m_size;
  }
  iterator insert(iterator pos, const T &value) {
    int ind = pos - begin();
    return insert(ind, value);
  }
  /**
   * inserts value at index ind.
   * after inserting, this->at(ind) == value
   * returns an iterator pointing to the inserted value.
   * throw index_out_of_bound if ind > size (in this situation ind can be size
   * because after inserting the size will increase 1.)
   */
  iterator insert(const size_t &ind, const T &value) {
    if (ind > m_size)
      throw;
    grow_capacity();
    if (m_size)
      std::construct_at(data + m_size, back());
    for (int i = (int)m_size - 1; i > ind; --i) {
      // m_size-1 ~ ind
      data[i] = data[i - 1];
    }
    data[ind] = value;
    ++m_size;
    return iterator(data + ind, data);
  }
  /**
   * removes the element at pos.
   * return an iterator pointing to the following element.
   * If the iterator pos refers the last element, the end() iterator is
   * returned.
   */
  iterator erase(iterator pos) {
    int p = pos - begin();
    for (int i = p; i < m_size; ++i)
      data[i] = data[i + 1];
    std::destroy_at(data + m_size - 1);
    --m_size;
    shrink_to_fit();
    return iterator(data + p, data);
  }
  /**
   * removes the element with index ind.
   * return an iterator pointing to the following element.
   * throw index_out_of_bound if ind >= size
   */
  iterator erase(const size_t &ind) {
    if (ind >= m_size)
      throw;
    return erase(iterator(data + ind, data));
  }
  /**
   * adds an element to the end.
   */
  void push_back(const T &value) {
    grow_capacity();
    std::construct_at(data + m_size, value);
    ++m_size;
  }
  /**
   * remove the last element from the end.
   * throw container_is_empty if size() == 0
   */
  void pop_back() {
    std::destroy_at(data + m_size - 1);
    --m_size;
    shrink_to_fit();
  }
};
struct string {
  char s[65];
  string() { s[0] = '\0'; }
  std::string to_string() {
    std::string tmp;
    for (int i = 0; s[i]; ++i)
      tmp.push_back(s[i]);
    return tmp;
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
std::ostream &operator<<(std::ostream &out, string &b) {
  out << b.s;
  return out;
}
} // namespace sjtu
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
};

#include <asm-generic/errno.h>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>

#include <istream>

#include <string>
#include <utility>

// new (&data) int(5);

// implement a b-plus-tree



namespace Bptree {

#define Bustub_PAGE_SIZE 16384
#define INTERNAL_PAGE_HEADER_SIZE 20
#define LEAF_PAGE_HEADER_SIZE 24

#define INDEX_TEMPLATE_ARGUMENTS                                               \
  template <typename KeyType, typename ValueType,                              \
            typename KeyComparator = std::less<KeyType>>

// define page type enum
enum class IndexPageType { INVALID_INDEX_PAGE = 0, LEAF_PAGE, INTERNAL_PAGE };
using page_id_t = int32_t;
using frame_id_t = int32_t;
struct RecycleList {
  // give next page_id
  page_id_t end_num_;
  std::fstream Recycle_filestream;
  void init() {
    Recycle_filestream.open("Recycle",
                            std::ios::binary | std::ios::in | std::ios::out);
    if (!Recycle_filestream.good()) {
      Recycle_filestream.clear();
      Recycle_filestream.open("Recycle", std::ios::binary | std::ios::trunc |
                                             std::ios::out | std::ios::in);
      end_num_ = 0;
    } else {
      Recycle_filestream.read(reinterpret_cast<char *>(&end_num_),
                              sizeof(page_id_t));
    }
    // std::cerr << end_num_ << std::endl;
  }
  ~RecycleList() {
    // std::cerr << "write " << end_num_ << std::endl;
    Recycle_filestream.seekp(0);
    Recycle_filestream.write(reinterpret_cast<const char *>(&end_num_),
                             sizeof(page_id_t));
    Recycle_filestream.close();
  }
  page_id_t GetVacancy() { return ++end_num_; }
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

    dustbin.init();
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
    // std::cerr << "filesize =  " << GetFileSize() << std::endl;
  }
  void WritePage(page_id_t page_id, const char *page_data,
                 const char *type_data, int def_size) {
    db_io_.seekp((page_id - 1) * Bustub_PAGE_SIZE);
    // std::cerr << page_id << " pointer " << db_io_.tellp() << std::endl;
    db_io_.write(type_data, OFFSET);
    db_io_.write(page_data, def_size);
  }
  void ReadType(page_id_t page_id, char *type_data, int def_size = 4) {
    assert(page_id != 0);
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
  // implement this function
  std::fstream db_io_;
  std::string file_name_;
  int num_flushes_{0};
  int num_writes_{0};
};

class BPlusTreeHeaderPage {
public:
  // Delete all constructor / destructor to ensure memory safety
  BPlusTreeHeaderPage() = delete;
  BPlusTreeHeaderPage(const BPlusTreeHeaderPage &other) = delete;
  page_id_t root_page_id_;
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
  ((Bustub_PAGE_SIZE - INTERNAL_PAGE_HEADER_SIZE) / (sizeof(MappingType)))
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

    auto KeyAt(int index) const -> KeyType { return array_[index].first; }
    /*
      return a pair first:pos, second: page_id_t
    */
    pair<size_t, page_id_t> find(const KeyType &key) {
      size_t pos = 0, sz = GetSize();
      for(int length = (1 << int(log2(sz))); length; length >>= 1)
        if (pos + length < sz && !(key < KeyAt(pos + length)))
          pos += length;
      return {pos, array_[pos].second};
    }
    /*  array_[index] = pair{key, val}
        顺次移动
    */
    void ModKeyAt(const KeyType &key, int index) { array_[index].first = key; }
    void SetKeyAt(int index, const KeyType &key, const ValueType &val) {
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
    auto ValueAt(int index) const -> page_id_t { return array_[index].second; }
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
    MappingType array_[INTERNAL_PAGE_SIZE];
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
      for(int length = (1 << int(log2(sz))); length; length >>= 1)
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
  struct BufferPoolManager {
#define MaxSize 1000
    DiskManager disk;
    size_t sz = 0;
    int pos[MaxSize], *rt;
    Ptr tmp[MaxSize];
    std::fstream root;

    void init(std::string file_name, int *header_id) {
      memset(pos, 0, sizeof(pos));
      this->disk.init(file_name);
      rt = header_id;
      root.open("root", std::ios::binary | std::ios::in | std::ios::out);
      if (!root.good()) {
        root.clear();
        root.open("root", std::ios::binary | std::ios::trunc | std::ios::out |
                              std::ios::in);
        *header_id = -1;
      } else {
        root.read(reinterpret_cast<char *>(header_id), sizeof(int));
      }
    }
    void getpos(PtrHead *head) { head->pos = disk.dustbin.GetVacancy(); }
    void setpos(PtrHead *head, int pos_) { head->pos = pos_; }

    void WritePage(PtrHead *head, BPlusTreePage *content) {

      IndexPageType ind = content->IsLeafPage() ? IndexPageType::LEAF_PAGE
                                                : IndexPageType::INTERNAL_PAGE;
      assert(head->pin == 1);
      if (ind == IndexPageType::LEAF_PAGE)
        disk.WritePage(head->pos, reinterpret_cast<const char *>(content),
                       reinterpret_cast<const char *>(&ind),
                       sizeof(BPlusTreeLeafPage));
      else
        disk.WritePage(head->pos, reinterpret_cast<const char *>(content),
                       reinterpret_cast<const char *>(&ind),
                       sizeof(BPlusTreeInternalPage));
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
      // auto to_string = [&](BPlusTreePage *node) -> std::string {
      //   if (node->IsLeafPage())
      //     return static_cast<BPlusTreeLeafPage *>(node)->ToString();
      //   else
      //     return static_cast<BPlusTreeInternalPage *>(node)->ToString();
      // };
      // std::cerr << "del "
      //             << pos_ << " " << tmp[pos_].head->pos << " "
      //             << tmp[pos_].head->pin << " " <<
      //             to_string(tmp[pos_].content) << tmp[pos_].head->count
      //                                                      << std::endl;
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
    }

    void get_content(page_id_t pos_, Ptr *pointer) {
      if (pos_ == -1)
        goto notfound;
      for (int i = 0; i < sz; ++i)
        if (pos_ == pos[i]) {
          *pointer = tmp[i];
          // std::cout << "get content for " << pos_ << " " << tmp[i].content
          //           << " " << tmp[i].head->count << std::endl;
          return;
        }
    notfound:;
      // auto to_string = [&](BPlusTreePage *node) -> std::string {
      //   if (node->IsLeafPage())
      //     return static_cast<BPlusTreeLeafPage *>(node)->ToString();
      //   else
      //     return static_cast<BPlusTreeInternalPage *>(node)->ToString();
      // };
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

    ~BufferPoolManager() {
      for (size_t i = 0; i < sz; ++i) {
        if (pos[i])
          // assert(tmp[i].head->count == 1);
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
    BufferPoolManager *bpm = nullptr;
    Ptr() = default;
    void getpos() { bpm->getpos(head); }
    Ptr(BufferPoolManager *bpm_, BPlusTreePage *cont = nullptr, int pos_ = -1) {
      if (pos_) {
        bpm = bpm_;
        content = cont;
        bpm->get_content(pos_, this);
        assert(content != nullptr);
      }
    }
    Ptr(const Ptr &rhs) {
      content = rhs.content;
      head = rhs.head;
      bpm = rhs.bpm;
      ++head->count;
    }
    void WritePage() { bpm->WritePage(head, content); }
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
  BPlusTree(std::string disk_name)
      : leaf_max_size_(LEAF_PAGE_SIZE), internal_max_size_(INTERNAL_PAGE_SIZE) {
    // page_id manager?
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
  // Insert a key-value pair into this B+ tree.
  void SetKeyAt(BPlusTreePage *page, const KeyType &key, const ValueType &val,
                int index) {
    if (page->IsLeafPage())
      static_cast<BPlusTreeLeafPage *>(page)->SetKeyAt(index, key, val);
    else
      static_cast<BPlusTreeInternalPage *>(page)->SetKeyAt(index, key, val);
  }
  KeyType KeyAt(BPlusTreePage *page, int index) {
    if (page->IsLeafPage())
      return static_cast<BPlusTreeLeafPage *>(page)->KeyAt(index);
    return static_cast<BPlusTreeInternalPage *>(page)->KeyAt(index);
  }
  ValueType ValAt(BPlusTreePage *page, int index) {
    if (page->IsLeafPage())
      return static_cast<BPlusTreeLeafPage *>(page)->ValAt(index);
    else
      return static_cast<BPlusTreeInternalPage *>(page)->ValueAt(index);
  }
  void DelAt(BPlusTreePage *page, int index) {
    if (page->IsLeafPage())
      static_cast<BPlusTreeLeafPage *>(page)->DelAt(index);
    else
      static_cast<BPlusTreeInternalPage *>(page)->DelAt(index);
  }
  BPlusTreeLeafPage *weiling = nullptr;
  bool Insert(const KeyType &key, const ValueType &value) {

    sjtu::vector<Ptr> v;
    sjtu::vector<int> v1;
    FindPosVector(key, &v, &v1);
    // std::cerr << "Insert " << key.second << " ";
    //  for (auto i : v)
    //    std::cerr << to_string(i.content) << "|";
    //  std::cerr << std::endl;
    /*
      case 0
      empty
    */
    if (v.empty()) {
      BPlusTreeLeafPage *new_page = new BPlusTreeLeafPage;
      new_page->SetKeyAt(0, key, value);
      new_page->SetNextPageId(0);
      weiling = new_page;
      Ptr pos(&bpm, new_page);
      pos.Dirty();
      header_page_id_ = pos.head->pos;
      return 1;
    }
    // assert(v.back().head->count == 2);
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
    if (new_page->next_page_id_ == 0)
      weiling = new_page;
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
                         fa_cont->ValueAt(i));
      fa->SetSize(internal_max_size_ >> 1);
      if (index < (internal_max_size_ >> 1)) {
        fa_cont->SetKeyAt(index, KeyAt(new1_page, 0), new_pos.head->pos);
      } else {
        new_fa->SetKeyAt(index - (internal_max_size_ >> 1), KeyAt(new1_page, 0),
                         new_pos.head->pos);
      }
      Ptr new_fa_page(&bpm, new_fa);
      fa.Dirty(), new_fa_page.Dirty();
      // std::cerr << "fa " << to_string(fa.content) << std::endl;
      // std::cerr << "new_fa " << to_string(new_fa) << std::endl;

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
        FindPosVector(key, &v, &v1);
        return 1;
      }
    }

    return 1;
  }
  void update(Ptr &fa, BPlusTreePage *fa_cont, Ptr &pre,
              BPlusTreePage *pre_cont, Ptr &nxt, BPlusTreePage *nxt_cont,
              size_t index, int dx) {
    if (dx > 0) {
      // bro = pos_cont + 1 bro.first = pos_cont.last
      auto res = MappingType{KeyAt(nxt_cont, 0), ValAt(nxt_cont, 0)};
      SetKeyAt(pre_cont, res.first, res.second, pre_cont->GetSize());
      DelAt(nxt_cont, 0);
    } else {
      // pos_cont = bro + 1
      auto res = MappingType{KeyAt(pre_cont, pre_cont->GetSize() - 1),
                             ValAt(pre_cont, pre_cont->GetSize() - 1)};
      SetKeyAt(nxt_cont, res.first, res.second, 0);
      DelAt(pre_cont, pre_cont->GetSize() - 1);
    }
    static_cast<BPlusTreeInternalPage *>(fa_cont)->ModKeyAt(KeyAt(nxt_cont, 0),
                                                            index + 1);
    fa.Dirty(), pre.Dirty(), nxt.Dirty();
  }

  void merge(Ptr &fa, BPlusTreePage *fa_cont, Ptr &pre, BPlusTreePage *pre_cont,
             Ptr &nxt, BPlusTreePage *nxt_cont, size_t index) {
    size_t offset = pre_cont->GetSize();
    for (int i = 0; i < nxt_cont->GetSize(); ++i)
      SetKeyAt(pre_cont, KeyAt(nxt_cont, i), ValAt(nxt_cont, i), offset + i);
    if (pre_cont->IsLeafPage())
      static_cast<BPlusTreeLeafPage *>(pre_cont)->SetNextPageId(
          static_cast<BPlusTreeLeafPage *>(nxt_cont)->GetNextPageId());
    // printtree(Ptr(&bpm, nullptr, header_page_id_));
    if (nxt_cont == weiling)
      weiling = static_cast<BPlusTreeLeafPage *>(pre_cont);
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
        bro = Ptr(&bpm, nullptr, fa_cont->ValueAt(index1 + 1)), flag = 1;
      else
        bro = Ptr(&bpm, nullptr, fa_cont->ValueAt(index1 - 1)), flag = -1;
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
      header_page_id_ = ValAt(pos.content, 0);
      // std::cerr << "删掉父亲"  << std::endl;
      return;
    }
    if (cont->GetSize() == 0)
      header_page_id_ = -1;
  }
  // Return the value associated with a given key
  void GetValue(const unsigned long long &key, sjtu::vector<ValueType> *result) {
    // if(header_page_id_)
    // printtree(Ptr(&bpm, nullptr, header_page_id_));
    // static int time = 0;
    // if(++time == 14)
    //   printtree();
    Ptr l = FindPos(KeyType{key, INT_MIN});
    BPlusTreeLeafPage *tmp;
    int id = -1;
    // printtree();
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
  BufferPoolManager bpm;
  void debug() {}

private:
  /* Debug Routines for FREE!! */
  // member variable
  sjtu::string index_name_;
  int leaf_max_size_;
  int internal_max_size_;
  page_id_t header_page_id_ = -1;
};

} // namespace Bptree

using pairtype = pair<unsigned long long, int>;
sjtu::vector<int> result;
Bptree::BPlusTree<pair<unsigned long long, int>, int> Bpt("disk");

// void test() {
//   srand(time(0));
//   Bpt.debug();
//   sjtu::string s("1"), s1("11");
// #define fr(i, a, b) for (int i = a; i <= b; ++i)
// #define rf(i, a, b) for (int i = a; i >= b; --i)
//   int a[10000];
//   int range;
//   std::cin >> range;
//   fr(i, 0, range) std::cin >> a[i];
//   fr(i, 0, range) {
//     std::cout << "insert " << a[i] << std::endl;
//     Bpt.printtree();
//     Bpt.Insert({s1, a[i]}, a[i]);
//     sjtu::vector<int> result;
//     if (a[i] == 70)
//       std::cout << 233 << std::endl;
//     Bpt.GetValue(s1, &result);
//     // for (auto i : result) std::cout << i << " ";std::cerr << std::endl;
//     std::cerr << a[i] << " num = " << result.size() << std::endl;
//   }
//   // std::random_shuffle(a,a+100);
//   Bpt.printtree();
//   rf(i, range, 0) {
//     sjtu::vector<int> result;
//     // if(a[i]%2)Bpt.Insert({s, a[i]}, 100 - a[i]);
//     // else
//     if (a[i] == 77)
//       std::cerr << 233;
//     // Bpt.printtree();
//     Bpt.Remove({s1, a[i]});
//     Bpt.GetValue(s1, &result);
//     // for (auto i : result) std::cout << i << " ";std::cerr << std::endl;
//     std::cerr << a[i] << " num = " << result.size() << std::endl;
//     // Bpt.printtree();
//   }
//   // Bpt.Insert({s + s,2},2);
// }
int main(int, char **) {
  // freopen("in.txt", "r", stdin);
  // freopen("output.txt","w",stdout);
  //  freopen("output.txt","w",stdout);
  // //  // delete file page and Recycle
  // test();
  //  return 0;

  std::ios::sync_with_stdio(false);
  std::cin.tie(0), std::cout.tie(0);
  int n, val;
  std::cin >> n;
  sjtu::string com;
  // std::cerr << Bpt.GetRootPageId() << std::endl;
  // Bpt.printtree();
  auto gethsh = [&](sjtu::string a, unsigned long long &b) -> void {
    int sz = a.size();
    for (int i = 0; i < sz; ++i)
      b = b * 131 + a[i];
  };
  for (int i = 1; i <= n; ++i) {
    std::cin >> com.s;
    // std::cerr << i << std::endl;
    //  if(i == 6)
    //    std::cerr << Bpt.weiling << std::endl;
    //  assert(Bpt.weiling == nullptr || Bpt.weiling->GetNextPageId() == 0);
    // Bpt.printtree();
    //  std::cerr << i << std::endl;
    unsigned long long hsh = 0;
    if (com == "insert") {
      // Bpt.
      std::cin >> com.s >> val;
      gethsh(com, hsh);
      Bpt.Insert(pairtype(hsh, val), val);
      // Bpt.printtree();
    } else if (com == "find") {
      std::cin >> com.s;
      gethsh(com, hsh);
      Bpt.GetValue(hsh, &result);
      // std::cerr << result.size() << std::endl;
      for (auto i : result)
        std::cout << i << " ";
      if (result.empty())
        std::cout << "null\n";
      else
        std::cout << "\n"; // assert(result.size() + (i-1000)/2 == 1000);

    } else {
      std::cin >> com.s >> val;
      gethsh(com, hsh);
      Bpt.Remove(pairtype(hsh, val));
    }
  }
  // Bpt.printtree();
}

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <string.h>
#include <string>
#include <unordered_map>
#include <utility>
// new (&data) int(5);
namespace sjtu {
template <typename T> class vector {
  size_t m_size, m_cap;
  T *data;
  std::allocator<T> a;

public:
  /**
   * TODO
   * a type for actions of the elements of a vector, and you should write
   *   a class named const_iterator with same interfaces.
   */
  /**
   * you can see RandomAccessIterator at CppReference for help.
   */
  class const_iterator;
  class iterator {
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a type.
    // For instance, for an iterator, iterator::value_type is the type that the
    // iterator points to.
    // STL algorithms and containers may use these type_traits (e.g. the
    // following typedef) to work properly. In particular, without the following
    // code,
    // @code{std::sort(iter, iter1);} would not compile.
    // See these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits
    // About value_type:
    // https://blog.csdn.net/u014299153/article/details/72419713 About
    // iterator_category: https://en.cppreference.com/w/cpp/iterator
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

  private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    pointer dat;
    pointer begin;

  public:
    iterator(pointer dat2 = nullptr, pointer begin2 = nullptr)
        : begin(begin2), dat(dat2){};
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
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
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
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
    std::construct_at(data + m_size, std::move(value));
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

} // namespace sjtu

// implement a b-plus-tree

/*
Your implementation will support thread-safe search,
insertion, deletion (including splitting and merging nodes), and an iterator to
support in-order leaf scans.
*/

namespace Bptree {

#define Bustub_PAGE_SIZE 4096
#define INTERNAL_PAGE_HEADER_SIZE 12
#define LEAF_PAGE_HEADER_SIZE 16

#define MappingType std::pair<KeyType, ValueType>

#define INDEX_TEMPLATE_ARGUMENTS                                               \
  template <typename KeyType, typename ValueType, typename KeyComparator = std::less<KeyType>>

// define page type enum
enum class IndexPageType { INVALID_INDEX_PAGE = 0, LEAF_PAGE, INTERNAL_PAGE };
using page_id_t = int32_t;
using frame_id_t = int32_t;

class DiskManager {
  friend class SharedPtr;

public:
  explicit DiskManager(const std::string &db_file) {
    this->file_name_ = db_file;
    db_io_.open(file_name_);
    if (!db_io_.good())
      db_io_.open(file_name_, std::ios::out);
    db_io_.close();
  }
  /**
   * Write a page to the database file.
   * @param page_id id of the page
   * @param page_data raw page data
   */
  class Recycle_List{
    
  };
  page_id_t WritePage(const char* page_data) {
  }
  
  void WritePage(page_id_t page_id, const char *page_data) {
    db_io_.open(file_name_);
    db_io_.seekp((page_id - 1) * Bustub_PAGE_SIZE);
    db_io_.write(page_data, Bustub_PAGE_SIZE);
    db_io_.close();
  }
  void ReadPage(page_id_t page_id, char *page_data) {
    db_io_.open(file_name_);
    db_io_.seekg((page_id - 1) * Bustub_PAGE_SIZE);
    db_io_.read(page_data, Bustub_PAGE_SIZE);
    db_io_.close();
  }
protected:
    // implement this function
  auto GetFileSize(const std::string &file_name) -> int {
    std::ifstream file(file_name, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
      return -1; // Return -1 if file cannot be opened
    }
    int file_size = file.tellg();
    file.close();
    return file_size;
  }
  std::fstream db_io_;
  std::string file_name_;
  int num_flushes_{0};
  int num_writes_{0};
} disk("page");

class BPlusTreeHeaderPage {
public:
  // Delete all constructor / destructor to ensure memory safety
  BPlusTreeHeaderPage() = delete;
  BPlusTreeHeaderPage(const BPlusTreeHeaderPage &other) = delete;
  page_id_t root_page_id_;
};
class BPlusTreePage {
public:
  // Delete all constructor / destructor to ensure memory safety
  BPlusTreePage() = delete;
  BPlusTreePage(const BPlusTreePage &other) = delete;
  ~BPlusTreePage() = delete;
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

private:
  // Member variables, attributes that both internal and leaf page share
  IndexPageType page_type_;
  int size_;
  int max_size_;
};

class BufferPoolManager {
public:
  /**
   * @brief Creates a new BufferPoolManager.
   * @param pool_size the size of the buffer pool
   * @param disk_manager the disk manager
   * @param replacer_k the LookBack constant k for the LRU-K replacer
   * @param log_manager the log manager (for testing only: nullptr = disable
   * logging). Please ignore this for P1.
   */
  class Page {
    // There is book-keeping information inside the page that should only be
    // relevant to the buffer pool manager.
    friend class BufferPoolManager;

  public:
    /** Constructor. Zeros out the page data. */
    Page() {
      data_ = new char[Bustub_PAGE_SIZE];
      ResetMemory();
    }

    /** Default destructor. */
    ~Page() { delete[] data_; }

    /** @return the actual data contained within this page */
    inline auto GetData() -> char * { return data_; }

    /** @return the page id of this page */
    inline auto GetPageId() -> page_id_t { return page_id_; }

  protected:
    static_assert(sizeof(page_id_t) == 4);
    // static_assert(sizeof(lsn_t) == 4);
    static constexpr size_t SIZE_PAGE_HEADER = 8;
    static constexpr size_t OFFSET_PAGE_START = 0;
    static constexpr size_t OFFSET_LSN = 4;

  private:
    /** Zeroes out the data that is held within the page. */
    inline void ResetMemory() {
      memset(data_, OFFSET_PAGE_START, Bustub_PAGE_SIZE);
    }

    /** The actual data that is stored within a page. */
    // Usually this should be stored as `char data_[BUSTUB_PAGE_SIZE]{};`. But
    // to enable ASAN to detect page overflow, we store it as a ptr.
    char *data_;
    /** The ID of this page. */
    page_id_t page_id_ = 0;
    /** The pin count of this page. */
    int pin_count_ = 0;
    /** True if the page is dirty, i.e. it is different from its corresponding
     * page on disk. */
    bool is_dirty_ = false;
    /** Page latch. */
    // ReaderWriterLatch rwlatch_;
  };
  BufferPoolManager(size_t pool_size, DiskManager *disk_manager,
                    size_t replacer_k = 50);

  /**
   * @brief Destroy an existing BufferPoolManager.
   */
  ~BufferPoolManager();

  /** @brief Return the size (number of frames) of the buffer pool. */
  auto GetPoolSize() -> size_t { return pool_size_; }

  /** @brief Return the pointer to all the pages in the buffer pool. */
  auto GetPages() -> Page * { return pages_; }

  /**
   * TODO(P1): Add implementation
   *
   * @brief Create a new page in the buffer pool. Set page_id to the new page's
   * id, or nullptr if all frames are currently in use and not evictable (in
   * another word, pinned).
   *
   * You should pick the replacement frame from either the free list or the
   * replacer (always find from the free list first), and then call the
   * AllocatePage() method to get a new page id. If the replacement frame has a
   * dirty page, you should write it back to the disk first. You also need to
   * reset the memory and metadata for the new page.
   *
   * Remember to "Pin" the frame by calling replacer.SetEvictable(frame_id,
   * false) so that the replacer wouldn't evict the frame before the buffer pool
   * manager "Unpin"s it. Also, remember to record the access history of the
   * frame in the replacer for the lru-k algorithm to work.
   *
   * @param[out] page_id id of created page
   * @return nullptr if no new pages could be created, otherwise pointer to new
   * page
   */
  auto NewPage(page_id_t *page_id) -> Page *;

  /**
   * TODO(P1): Add implementation
   *
   * @brief PageGuard wrapper for NewPage
   *
   * Functionality should be the same as NewPage, except that
   * instead of returning a pointer to a page, you return a
   * BasicPageGuard structure.
   *
   * @param[out] page_id, the id of the new page
   * @return BasicPageGuard holding a new page
   */
  // auto NewPageGuarded(page_id_t *page_id) -> BasicPageGuard;

  /**
   * TODO(P1): Add implementation
   *
   * @brief Fetch the requested page from the buffer pool. Return nullptr if
   * page_id needs to be fetched from the disk but all frames are currently in
   * use and not evictable (in another word, pinned).
   *
   * First search for page_id in the buffer pool. If not found, pick a
   * replacement frame from either the free list or the replacer (always find
   * from the free list first), read the page from disk by scheduling a read
   * DiskRequest with disk_scheduler_->Schedule(), and replace the old page in
   * the frame. Similar to NewPage(), if the old page is dirty, you need to
   * write it back to disk and update the metadata of the new page
   *
   * In addition, remember to disable eviction and record the access history of
   * the frame like you did for NewPage().
   *
   * @param page_id id of page to be fetched
   * @param access_type type of access to the page, only needed for leaderboard
   * tests.
   * @return nullptr if page_id cannot be fetched, otherwise pointer to the
   * requested page
   */
  auto FetchPage(page_id_t page_id) -> Page *;

  /**
   * TODO(P1): Add implementation
   *
   * @brief PageGuard wrappers for FetchPage
   *
   * Functionality should be the same as FetchPage, except
   * that, depending on the function called, a guard is returned.
   * If FetchPageRead or FetchPageWrite is called, it is expected that
   * the returned page already has a read or write latch held, respectively.
   *
   * @param page_id, the id of the page to fetch
   * @return PageGuard holding the fetched page
   */
  // auto FetchPageBasic(page_id_t page_id) -> BasicPageGuard;
  // auto FetchPageRead(page_id_t page_id) -> ReadPageGuard;
  // auto FetchPageWrite(page_id_t page_id) -> WritePageGuard;

  /**
   * TODO(P1): Add implementation
   *
   * @brief Unpin the target page from the buffer pool. If page_id is not in the
   * buffer pool or its pin count is already 0, return false.
   *
   * Decrement the pin count of a page. If the pin count reaches 0, the frame
   * should be evictable by the replacer. Also, set the dirty flag on the page
   * to indicate if the page was modified.
   *
   * @param page_id id of page to be unpinned
   * @param is_dirty true if the page should be marked as dirty, false otherwise
   * @param access_type type of access to the page, only needed for leaderboard
   * tests.
   * @return false if the page is not in the page table or its pin count is <= 0
   * before this call, true otherwise
   */
  // auto UnpinPage(page_id_t page_id, bool is_dirty,
  //                AccessType access_type = AccessType::Unknown) -> bool;

  /**
   * TODO(P1): Add implementation
   *
   * @brief Flush the target page to disk.
   *
   * Use the DiskManager::WritePage() method to flush a page to disk, REGARDLESS
   * of the dirty flag. Unset the dirty flag of the page after flushing.
   *
   * @param page_id id of page to be flushed, cannot be INVALID_PAGE_ID
   * @return false if the page could not be found in the page table, true
   * otherwise
   */
  auto FlushPage(page_id_t page_id) -> bool;

  /**
   * TODO(P1): Add implementation
   *
   * @brief Flush all the pages in the buffer pool to disk.
   */
  void FlushAllPages();

  /**
   * TODO(P1): Add implementation
   *
   * @brief Delete a page from the buffer pool. If page_id is not in the buffer
   * pool, do nothing and return true. If the page is pinned and cannot be
   * deleted, return false immediately.
   *
   * After deleting the page from the page table, stop tracking the frame in the
   * replacer and add the frame back to the free list. Also, reset the page's
   * memory and metadata. Finally, you should call DeallocatePage() to imitate
   * freeing the page on the disk.
   *
   * @param page_id id of page to be deleted
   * @return false if the page exists but could not be deleted, true if the page
   * didn't exist or deletion succeeded
   */
  auto DeletePage(page_id_t page_id) -> bool;

private:
  /** Number of pages in the buffer pool. */
  const size_t pool_size_;
  /** The next page id to be allocated  */
  page_id_t next_page_id_ = 0;

  /** Array of buffer pool pages. */
  Page *pages_;
  /** Pointer to the disk sheduler. */
  // std::unique_ptr<DiskScheduler> disk_scheduler_ __attribute__((__unused__));
  /** Pointer to the log manager. Please ignore this for P1. */
  // LogManager *log_manager_ __attribute__((__unused__));
  /** Page table for keeping track of buffer pool pages. */
  std::unordered_map<page_id_t, frame_id_t> page_table_;
  /** Replacer to find unpinned pages for replacement. */
  // std::unique_ptr<LRUKReplacer> replacer_;
  /**
   * @brief Allocate a page on disk. Caller should acquire the latch before
   * calling this function.
   * @return the id of the allocated page
   */
  auto AllocatePage() -> page_id_t;

  /**
   * @brief Deallocate a page on disk. Caller should acquire the latch before
   * calling this function.
   * @param page_id id of the page to deallocate
   */
  void DeallocatePage(__attribute__((unused)) page_id_t page_id) {
    // This is a no-nop right now without a more complex data structure to track
    // deallocated pages
  }

  // TODO(student): You may add additional private members and helper functions
};

INDEX_TEMPLATE_ARGUMENTS
class BPlusTree {
#define INTERNAL_PAGE_SIZE                                                     \
  ((Bustub_PAGE_SIZE - INTERNAL_PAGE_HEADER_SIZE) / (sizeof(MappingType)))
#define B_PLUS_TREE_LEAF_PAGE_TYPE                                             \
  BPlusTreeLeafPage<KeyType, ValueType, KeyComparator>
#define LEAF_PAGE_SIZE                                                         \
  ((Bustub_PAGE_SIZE - LEAF_PAGE_HEADER_SIZE) / sizeof(MappingType))

  class BPlusTreeInternalPage : public BPlusTreePage {
  public:
    // Delete all constructor / destructor to ensure memory safety
    BPlusTreeInternalPage() = delete;
    BPlusTreeInternalPage(const BPlusTreeInternalPage &other) = delete;
    /**
     * Writes the necessary header information to a newly created page, must be
     * called after the creation of a new page to make a valid
     * `BPlusTreeInternalPage`
     * @param max_size Maximal size of the page
     */
    void Init(int max_size = INTERNAL_PAGE_SIZE) {
      array_ = new MappingType[max_size];
      SetMaxSize(max_size);
      SetPageType(IndexPageType::INTERNAL_PAGE);
      SetSize(1);
    }
    /**
     * @param index The index of the key to get. Index must be non-zero.
     * @return Key at index
     */
    auto KeyAt(int index) const -> KeyType { return array_[index].first; }
    /**
     * @param index The index of the key to set. Index must be non-zero.
     * @param key The new value for key
     */
    void SetKeyAt(int index, const KeyType &key) { array_[index].first = key; }
    /**
     * @param value The value to search for
     * @return The index that corresponds to the specified value
     */
    auto ValueIndex(const ValueType &value) const -> int {
      for (int i = 1; i < GetSize(); ++i)
        if (KeyComparator{}(array_[i].second, value))
          return i;
    }

    /**
     * @param index The index to search for
     * @return The value at the index
     */
    auto ValueAt(int index) const -> ValueType { return array_[index].second; }

    /**
     * @brief For test only, return a string representing all keys in
     * this internal page, formatted as "(key1,key2,key3,...)"
     *
     * @return The string representation of all keys in the current internal
     * page
     */
    auto ToString() const -> std::string {
      std::string kstr = "(";
      bool first = true;

      // First key of internal page is always invalid
      for (int i = 1; i < GetSize(); i++) {
        KeyType key = KeyAt(i);
        if (first) {
          first = false;
        } else {
          kstr.append(",");
        }

        kstr.append(std::to_string(key.ToString()));
      }
      kstr.append(")");

      return kstr;
    }

  private:
    // Flexible array member for page data.
    MappingType *array_;
  };
  class BPlusTreeLeafPage : public BPlusTreePage {
  public:
    // Delete all constructor / destructor to ensure memory safety
    BPlusTreeLeafPage() = delete;
    BPlusTreeLeafPage(const BPlusTreeLeafPage &other) = delete;

    /**
     * After creating a new leaf page from buffer pool, must call initialize
     * method to set default values
     * @param max_size Max size of the leaf node
     */
    void Init(int max_size = LEAF_PAGE_SIZE) {
      array_ = new MappingType[max_size];
      SetMaxSize(max_size);
      SetPageType(IndexPageType::LEAF_PAGE);
      SetSize(1);
    }

    // Helper methods
    auto GetNextPageId() const -> page_id_t { return next_page_id_; }
    void SetNextPageId(page_id_t next_page_id) { next_page_id_ = next_page_id; }
    auto KeyAt(int index) const -> KeyType { return array_[index].first; }

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

        kstr.append(std::to_string(key.ToString()));
      }
      kstr.append(")");

      return kstr;
    }

  private:
    page_id_t next_page_id_;
    // Flexible array member for page data.
    MappingType *array_;
  };

public:
  explicit BPlusTree(
                     const KeyComparator& comparator,
                     int leaf_max_size = LEAF_PAGE_SIZE,
                     int internal_max_size = INTERNAL_PAGE_SIZE)
      : 
        comparator_(std::move(comparator)), leaf_max_size_(leaf_max_size),
        internal_max_size_(internal_max_size) {
    // page_id manager?
      
  }

  // Insert a key-value pair into this B+ tree.
  bool Insert(const KeyType &key, const ValueType &value) {
    return 1;
  }
  // Remove a key and its value from this B+ tree.
  void Remove(const KeyType &key) {}
  // Return the value associated with a given key
  bool GetValue(const KeyType &key, sjtu::vector<ValueType> *result) {}

  // Return the page id of the root node
  page_id_t GetRootPageId() { return header_page_id_; }

private:
  /* Debug Routines for FREE!! */
  // member variable
  std::string index_name_;
  KeyComparator comparator_;
  int leaf_max_size_;
  int internal_max_size_;
  page_id_t header_page_id_;
};

} // namespace Bptree

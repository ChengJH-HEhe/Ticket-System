#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "exceptions.hpp"

#include <iterator>
#include <memory>

#include <cstddef>
#include <strings.h>
#include <utility>

// new (&data) int(5);

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */

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
        throw invalid_iterator();
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
        throw invalid_iterator();
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
  vector() { m_size = 0, m_cap = 0; data = nullptr;}
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
      throw index_out_of_bound();
    return *(data + pos);
  }
  const T &at(const size_t &pos) const {
    if (pos >= m_size)
      throw index_out_of_bound();
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
      throw container_is_empty();
    return *data;
  }
  /**
   * access the last element.
   * throw container_is_empty if size == 0
   */
  const T &back() const {
    if (empty())
      throw container_is_empty();
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
    if(ind > m_size) throw index_out_of_bound();
    grow_capacity();
    if (m_size)
      std::construct_at(data + m_size, back());
    for (int i = (int)m_size - 1; i > ind; --i) {
      // m_size-1 ~ ind
      data[i] = std::move(data[i - 1]);
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
      throw index_out_of_bound();
    return erase(iterator(data + ind, data));
  }
  /**
   * adds an element to the end.
   */
  void push_back(const T &value) {
    grow_capacity();
    std::construct_at(data+m_size , std::move(value));
    ++m_size;
  }
  /**
   * remove the last element from the end.
   * throw container_is_empty if size() == 0
   */
  void pop_back() { 
    std::destroy_at(data+m_size-1);
    --m_size;
    shrink_to_fit();
  }
};

} // namespace sjtu

#endif

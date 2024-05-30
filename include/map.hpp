/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include "exceptions.hpp"
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include "pair.hpp"

namespace sjtu {

template <class Key, class T, class Compare = std::less<Key>> class map {
  enum colorT { RED, BLACK };
  struct _rb_node {
    pair<Key, T> data;
    typedef pair<Key, T> value_type;
    _rb_node *left, *right, *parent;
    colorT color;
    _rb_node(value_type pr, _rb_node *l = nullptr, _rb_node *r = nullptr,
             _rb_node *p = nullptr, colorT col = RED)
        : data(std::move(pr)), left(l), right(r), parent(p), color(col){};
    _rb_node(value_type pr, _rb_node *p, colorT col, _rb_node *l = nullptr,
             _rb_node *r = nullptr)
        : data(std::move(pr)), parent(std::move(p)), color(col), left(l),
          right(r){};
    _rb_node *pre() const {
      if (left) {
        auto nw = left;
        while (nw->right)
          nw = nw->right;
        return nw;
      } else {
        auto nw = this;
        while (nw->isLeft())
          nw = nw->parent;
        if (nw->isRoot())
          return nullptr;
        else
          return nw->parent;
      }
    }
    _rb_node *nxt() const {
      if (right) {
        auto nw = right;
        while (nw->left)
          nw = nw->left;
        return nw;
      } else {
        auto nw = this;
        while (nw->isRight())
          nw = nw->parent;
        if (nw->isRoot())
          return nullptr;
        else
          return nw->parent;
      }
    }
    inline bool isBlack() const { return this->color == BLACK; }
    inline bool isRed() const { return this->color == RED; }
    inline bool isLeft() const {
      return this->parent && this == this->parent->left;
    }
    inline bool isRight() const {
      return this->parent && this == this->parent->right;
    }
    inline bool isRoot() const { return parent == nullptr; }
  };
  _rb_node *root = nullptr;
  size_t idcount = 0;

  void buildTree(_rb_node *&nw, _rb_node *ex) {
    if (ex == nullptr)
      return;
    nw = new _rb_node(ex->data, nullptr, ex->color);
    buildTree(nw->left, ex->left);
    buildTree(nw->right, ex->right);
    maintainParent(nw);
  }

  void clearNode(_rb_node *rt) {
    if (rt->left)
      clearNode(rt->left);
    if (rt->right)
      clearNode(rt->right);
    delete rt;
  }
  void clearTree() {
    if (root) {
      clearNode(root), idcount = 0;
      root = nullptr;
    }
  }

  void maintainParent(_rb_node *x) {
    if (x->left)
      x->left->parent = x;
    if (x->right)
      x->right->parent = x;
  }
  void LL(_rb_node *gp) {
    _rb_node *p = gp->left;
    if (gp == root)
      root = p;
    gp->left = p->right;
    p->right = gp;
    if (gp->parent) {
      if(gp->isLeft())
        gp->parent->left = p;
      else
        gp->parent->right = p;
    }
    p->parent = gp->parent;
    std::swap(p->color, gp->color);
    maintainParent(p);
    maintainParent(gp);
    // 红红黑黑 染色
  }
  void RR(_rb_node *gp) {
    _rb_node *p = gp->right;
    if (gp == root)
      root = p;
    gp->right = p->left;
    p->left = gp;
    std::swap(p->color, gp->color);
    if (gp->parent) {
      if(gp->isLeft())
        gp->parent->left = p;
      else
        gp->parent->right = p;
    }
    p->parent = gp->parent;
    maintainParent(p);
    maintainParent(gp);
  }
  void LR(_rb_node *gp) {
    _rb_node *p = gp->left, *x = p->right;
    RR(p), LL(gp);
  }
  void RL(_rb_node *gp) {
    _rb_node *p = gp->right, *x = p->left;
    LL(p), RR(gp);
  }
  void insAdjust(_rb_node *&gp, _rb_node *&p, _rb_node *&x) {
    // gp p x != nullptr
    if (p == nullptr || p->isBlack())
      return;
    if (p == root) {
      p->color = BLACK;
      return;
    }
    if (p->isLeft()) {
      if (x->isLeft())
        LL(gp);
      else
        LR(gp);
    } else {
      if (x->isRight())
        RR(gp);
      else
        RL(gp);
    }
  }
  inline bool isequal(const Key &key1, const Key &key2) {
    return !Compare{}(key1, key2) && !Compare{}(key2, key1);
  }

  bool isblack(_rb_node *p) { return p == nullptr || p->isBlack(); }
  bool isred(_rb_node *p) { return p != nullptr && p->isRed(); }
  /*
  尽量调整当前节点的颜色为红色,并确信父亲为红，且父亲到儿子这条路径上的点都是合法染色的
  1.当前节点是根节点，且双儿子，颜色相同，自己红，儿子搞成全黑，进入下一层
  2.自己双儿子为黑
     .1 兄弟也全黑 父亲是红，可以重新然自己、兄弟、父亲
     .2
  */

  void rmvadjust(_rb_node *&p, _rb_node *&c, _rb_node *&b, const Key &val) {
    static int times = 0;
    if (c->color == RED)
      return;
    // let c->color = RED 删去后不影响
    if (c == root && c->right && c->left && c->left->color == c->right->color) {
      c->color = RED;
      c->left->color = c->right->color = BLACK;
      return;
    }

    if (isblack(c->left) && isblack(c->right)) {
      if (isblack(b->left) && isblack(b->right)) {
        p->color = BLACK;
        b->color = c->color = RED;// b = RED c = RED
        // 兄弟也是全黑，可以直接染色
      } else {
        if (b->isLeft()) {
          // 兄弟至少有一个红节点
          if (isred(b->left)) {
            LL(p);
            p->color = b->left->color = BLACK;
            c->color = b->color = RED;
          } else {
            LR(p);
            p->color = b->color = BLACK;
            c->color = b->parent->color = RED;
          }
        } else {
          if (isred(b->right)) {
            RR(p);
            p->color = b->right->color = BLACK;
            b->color = c->color = RED;
          } else {
            RL(p);
            p->color = b->color = BLACK;
            c->color = b->parent->color = RED;
            p = p->parent;
          }
        }
      }
    } else {
      if (isequal(c->data.first, val)) {
        if (c->left && c->right) {
          if (c->right->color == BLACK) {
            LL(c);
            p = c->parent;
          }
          return;
        }
        if (c->left) {
          LL(c);
          p = c->parent;
          assert(c->color == RED);
        } else {
          RR(c);
          p = c->parent;
          // root->left == root;
        }
      } else {
        p = c;
        c = (Compare{}(val, p->data.first) ? p->left : p->right);
        b = (c == p->left ? p->right : p->left);
        if (c->color == BLACK) {
          if (b == p->right)
            RR(p);
          else
            LL(p);
          // 
          b = c == p->left ? p->right : p->left;
          debug_without_assert();
          rmvadjust(p, c, b, val);
        }
      }
    }
  }
  // 改变边：
  // 不相邻时：指向儿子：维护儿子，对应的父亲；指向父亲，维护父亲的儿子

  void maintainChildren(_rb_node *suc, _rb_node *nw) {
    _rb_node *tmp_suc_parent = suc->parent, *tmp_nw_parent = nw->parent;
    if (tmp_suc_parent) {
      if (suc->isLeft())
        tmp_suc_parent->left = nw;
      else
        tmp_suc_parent->right = nw;
    } 
    if(tmp_nw_parent) {
      if (nw->isLeft())
        tmp_nw_parent->left = suc;
      else
        tmp_nw_parent->right = suc;
    }
    suc->parent = tmp_nw_parent, nw->parent = tmp_suc_parent;
  }

  void swap_node(_rb_node *nw, _rb_node *suc) {
    
    std::swap(suc->color, nw->color);
    if (suc == nw->right) {
      // 指针不能变， 边可以
      nw->right = suc->right, suc->right = nw;
      std::swap(suc->left, nw->left);
      if (nw == root) {
        suc->parent = nullptr;
        root = suc;
      } else {
        if (nw->isLeft())
          nw->parent->left = suc;
        else
          nw->parent->right = suc;
        suc->parent = nw->parent, nw->parent = suc;
      }
      maintainParent(nw), maintainParent(suc);
    } else {
      if(nw == root) root = suc;
      std::swap(suc->left, nw->left);
      std::swap(suc->right, nw->right);
      maintainParent(nw), maintainParent(suc);
      maintainChildren(suc, nw);
    }
  }
  void remove(const Key &key) {
    _rb_node *p, *c = root, *b;
    p = b = root;
    if (root == nullptr)
      return;
    if (isequal(root->data.first, key) && root->left == nullptr &&
        root->right == nullptr) {
      delete root;
      --idcount;
      root = nullptr;
      return;
    }
    // parent current brother
    while (1) {
      //std::cerr << "remove adjust for " << " " << c->data.second
      //         << '\n';
      rmvadjust(p, c, b, key);
      // if(isequal(c->data.first, key) && c->data.second == "2047")
      //   std::cerr<<c->data.second << std::endl;
      //assert((c->left && c->right) || c->color == RED);
      //print_without_assert(root);
      if (isequal(c->data.first, key)) {
        if (c->left && c->right) {
          _rb_node *tmp = c->nxt(); // 寻找下一个作为子节点
          swap_node(c, tmp); // 不改变节点内容，只改变边
          c = tmp->right;
          p = c->parent;
          b = c == p->left ? p->right : p->left;
          continue;
        } else {
          // 此时是红节点，必然没有儿子
          --idcount;
          assert(c != nullptr);
          if (c->parent) {
            if (c->isLeft())
              c->parent->left = nullptr;
            else
              c->parent->right = nullptr;
          }
          delete c;
          c = nullptr;
          root->color = BLACK;
          return;
        }
        
      }
      // 往下走一层
      p = c;
      c = Compare{}(key, p->data.first) ? p->left : p->right;
      b = c == p->left ? p->right : p->left;
      // if(!c)
      //   std::cerr << p->data.second << " " << p->left << " " << p->right << std::endl;
      assert(c);
    }
  }
  int ext = -1;
public:
  void print(_rb_node *nw, int dep = 0) {
    if (!nw) {
      if(ext > 0) assert(dep+1 == ext);
      ext = dep+1;
      return;
    }
    // std::cerr << nw << " " << nw->color << " " << (nw->data.second) << " "
    //           << nw->left << " " << nw->right << std::endl;
    if (nw->left)
      assert(nw->left->parent == nw), assert(nw->left != nw),
          assert(!(nw->left->color == RED && nw->color == RED));
    if (nw->right)
      assert(nw->right->parent == nw), assert(nw->right != nw),
          assert(!(nw->right->color == RED && nw->color == RED));
    
    print(nw->left, dep+(nw->color==BLACK)), print(nw->right,dep+(nw->color==BLACK));
  }
  void print_without_assert(_rb_node *nw) {
    return;
    if (!nw)
      return;
    std::cerr << nw << " " << nw->color << " " << (nw->data.second) << " "
              << nw->left << " " << nw->right << std::endl;
    // if(nw->left) assert(nw->left->parent == nw), assert(!(nw->left->color ==
    // RED && nw->color == RED)); if(nw->right) assert(nw->right->parent == nw),
    // assert(!(nw->right->color == RED && nw->color == RED));
    print_without_assert(nw->left), print_without_assert(nw->right);
  }

  void debug() {
    std::cerr << "______debug begin___________________\n";
    ext = -1;
    print(root);
  }
  void debug_without_assert() {
    // std::cerr << "______debug_without_assert begin___________________\n";
    // print_without_assert(root);
  }

  /**
   * the internal type of data.
   * it should have a default constructor, a copy constructor.
   * You can use sjtu::map as value_type by typedef.
   */
  typedef pair<Key, T> value_type;
  /**
   * see BidirectionalIterator at CppReference for help.
   *
   * if there is anything wrong throw invalid_iterator.
   *     like it = map.begin(); --it;
   *       or it = map.end(); ++end();
   */
  class const_iterator;
  class iterator {
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    _rb_node *dat;
    map<Key, T, Compare> *anc;
    friend class map<Key, T, Compare>;
    friend class const_iterator;

  public:
    _rb_node* data() {
      return dat;
    }
    iterator(_rb_node *da, map<Key, T, Compare> *an)
        : dat(std::move(da)), anc(std::move(an)) {
      // TODO
    }
    iterator(const iterator &other) {
      // TODO
      dat = other.dat;
      anc = other.anc;
    }
    iterator() = default;
    /**
     * TODO iter++
     */
    iterator operator++(int) {
      iterator tmp = *this;
      ++*this;
      return tmp;
    }
    /**
     * TODO ++iter
     */
    iterator &operator++() {
      if (dat == nullptr) {
        // ++end()
        throw invalid_iterator();
      }
      dat = dat->nxt();
      // end() 时 自然是 nullptr
      return *this;
    }
    /**
     * TODO iter--
     */
    iterator operator--(int) {
      iterator tmp(dat, anc);
      --*this;
      return tmp;
    }
    /**
     * TODO --iter
     */
    iterator &operator--() {
      // --end()
      if (dat == nullptr) {
        // tree is empty
        if (anc->root == nullptr)
          throw invalid_iterator();
        // otherwise find the last element
        else {
          _rb_node *content = anc->root;
          while (content->right)
            content = content->right;
          dat = content;
        }
      } else {
        dat = dat->pre();
        // --begin()
        if (dat == nullptr)
          throw invalid_iterator();
      }
      return *this;
    }
    value_type &operator*() { return dat->data; }
    const value_type &operator*() const { return dat->data; }

    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {
      return dat == rhs.dat && anc == rhs.anc;
    }
    bool operator==(const const_iterator &rhs) const {
      return dat == rhs.dat && anc == rhs.anc;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return !(dat == rhs.dat && anc == rhs.anc);
    }
    bool operator!=(const const_iterator &rhs) const {
      return !(dat == rhs.dat && anc == rhs.anc);
    }
    /**
     * for the support of it->first.
     * See
     * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
     * for help.
     */
    value_type *operator->() const noexcept { return &(dat->data); }
  };
  class const_iterator {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
  private:
    // data members.
    const _rb_node *dat;
    const map<Key, T, Compare> *anc;
    friend class iterator;
    friend class map<Key, T, Compare>;

  public:
    const_iterator() {
      // TODO
      dat = nullptr;
    }
    const_iterator(const const_iterator &other) {
      // TODO
      dat = other.dat;
    }
    const_iterator(const _rb_node *data, const map<Key, T, Compare> *an)
        : dat(std::move(data)), anc(std::move(an)){};
    const_iterator(const iterator &other) {
      // TODO
      dat = other.dat;
    }
    // And other methods in iterator.
    // And other methods in iterator.
    // And other methods in iterator.
    const_iterator operator++(int) {
      const_iterator tmp;
      tmp.dat = dat;
      ++*this;
      return tmp;
    }
    /**
     * TODO ++iter
     */
    const_iterator &operator++() {
      if (dat == nullptr) {
        // ++end()
        throw invalid_iterator();
      }
      dat = dat->nxt();
      // end() 时 自然是 nullptr
      return *this;
    }
    /**
     * TODO iter--
     */
    const_iterator operator--(int) {
      const_iterator tmp;
      tmp.dat = dat;
      --*this;
      return tmp;
    }
    /**
     * TODO --iter
     */
    const_iterator &operator--() {
      // --end()
      if (dat == nullptr) {
        // tree is empty
        if (anc->root == nullptr)
          throw invalid_iterator();
        // otherwise find the last element
        else {
          _rb_node *content = anc->root;
          while (content->right)
            content = content->right;
          dat = content;
        }
      } else {
        dat = dat->pre();
        // --begin()
        if (dat == nullptr)
          throw invalid_iterator();
      }
      return *this;
    }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    value_type &operator*() const {
      return const_cast<value_type &>(dat->data);
    }
    bool operator==(const iterator &rhs) const {
      return dat == rhs.dat && anc == rhs.anc;
    }
    bool operator==(const const_iterator &rhs) const {
      return dat == rhs.dat && anc == rhs.anc;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return !(dat == rhs.dat && anc == rhs.anc);
    }
    bool operator!=(const const_iterator &rhs) const {
      return !(dat == rhs.dat && anc == rhs.anc);
    }
    value_type *operator->() const noexcept {
      return const_cast<value_type *>(&dat->data);
    }
  };
  /**
   * TODO two constructors
   */
  // default constructor rt & sz
  // cannot bind non-const lvalue reference of type ‘const sjtu::map<Integer,
  // std::__cxx11::basic_string<char>, Compare>::_rb_node*&’ to a value of type
  // ‘std::nullptr_t’
  map() { // buildTree(root, nullptr);
    root = nullptr;
    idcount = 0;
  }
  map(const map &other) {
    buildTree(root, other.root);
    idcount = other.idcount;
  }
  /**
   * TODO assignment operator
   */
  map &operator=(const map &other) {
    if (this == &other)
      return *this;
    clearTree();
    buildTree(root, other.root);
    idcount = other.idcount;
    return *this;
  }
  /**
   * TODO Destructors
   */
  ~map() { clearTree(); }
  /**
   * TODO
   * access specified element with bounds checking
   * Returns a reference to the mapped value of the element with key equivalent
   * to key. If no such element exists, an exception of type
   * `index_out_of_bound'
   */
  T &at(const Key &key) {
    iterator ans = find(key);
    if (ans == end())
      throw index_out_of_bound();
    else
      return ans->second;
  }
  const T &at(const Key &key) const {
    const_iterator ans = find(key);
    if (ans == cend())
      throw index_out_of_bound();
    else
      return ans->second;
  }
  /**
   * TODO
   * access specified element
   * Returns a reference to the value that is mapped to a key equivalent to key,
   *   performing an insertion if such key does not already exist.
   */
  T &operator[](const Key &key) {
    iterator ans = find(key);
    if (ans == end())
      return insert(value_type(key, T())).first->second;
    else
      return ans->second;
    // ;
  }
  /**
   * behave like at() throw index_out_of_bound if such key does not exist.
   */
  const T &operator[](const Key &key) const {
    const_iterator ans = find(key);
    if (ans == cend())
      throw index_out_of_bound();
    else
      return ans->second;
  }
  /**
   * return a iterator to the beginning
   */
  iterator begin() {
    _rb_node *content = root;
    while (content && content->left)
      content = content->left;

    return iterator(content, this);
  }
  const_iterator cbegin() const {
    _rb_node *content = root;
    while (content && content->left)
      content = content->left;
    return const_iterator(content, this);
  }
  /**
   * return a iterator to the end
   * in fact, it returns past-the-end.
   */
  iterator end() { return iterator(nullptr, this); }
  const_iterator cend() const { return const_iterator(nullptr, this); }
  /**
   * checks whether the container is empty
   * return true if empty, otherwise false.
   */
  bool empty() const { return !idcount; }
  /**
   * returns the number of elements.
   */
  size_t getsz(_rb_node* nw) const{
    if(!nw) return 0;
    size_t tmp1 = getsz(nw->left);
    std::cerr<<nw->data.second<<" ";
    size_t tmp2 = getsz(nw->right);
    return  tmp1+tmp2+ 1;
  }
  size_t size() const { return idcount; }
  /**
   * clears the contents
   */
  void clear() { clearTree(); }
  /**
   * insert an element.
   * return a pair, the first of the pair is
   *   the iterator to the new element (or the element that prevented the
   * insertion), the second one is true if insert successfully, or false.
   */
  pair<iterator, bool> insert(const value_type &value) {
    _rb_node *p, *gp, *x;
    if (root == nullptr) {
      root = new _rb_node(value, nullptr, BLACK);
      ++idcount;
      return {iterator(root, this), 1};
    }
    p = gp = root, x = root;
    while (1) {
      if (x) {
        if (x->left && x->right && x->left->isRed() && x->right->isRed()) {
          x->left->color = x->right->color = BLACK;
          x->color = RED;
          insAdjust(gp, p, x);
        }
        gp = p, p = x;
        if (!(Compare{}(value.first, x->data.first) ||
              Compare{}(x->data.first, value.first)))
          return {iterator{x, this}, 0};
        x = Compare{}(value.first, x->data.first) ? x->left : x->right;
      } else {
        x = new _rb_node(value, p, RED);
        if (Compare{}(value.first, p->data.first))
          p->left = x;
        else
          p->right = x;
        // if(idcount == 4)
        //   std::cerr<<"__insadjust__\n",debug();
        insAdjust(gp, p, x);
        // debug();
        root->color = BLACK;
        ++idcount;
        return {iterator(x, this), 1};
      }
    }
  }
  /**
   * erase the element at pos.
   *
   * throw if pos pointed to a bad element (pos == this->end() || pos points an
   * element out of this)
   */
  void erase(iterator pos) {
    if (pos == this->end() || pos.anc != this)
      throw invalid_iterator{};
    remove(pos->first);
  }
  /**
   * Returns the number of elements with key
   *   that compares equivalent to the specified argument,
   *   which is either 1 or 0
   *     since this container does not allow duplicates.
   * The default method of check the equivalence is !(a < b || b > a)
   */
  size_t count(const Key &key) const {
    return (find(key) == cend()) ? 0 : 1;
    // WHY?
  }
  /**
   * Finds an element with key equivalent to key.
   * key value of the element to search for.
   * Iterator to an element with key equivalent to key.
   *   If no such element is found, past-the-end (see end()) iterator is
   * returned.
   */
  iterator find(const Key &key) {
    if(key == -1 || !root) return end();
    auto nw = root;
    while (nw &&
           (Compare{}(key, nw->data.first) || Compare{}(nw->data.first, key)))
      if (Compare{}(key, nw->data.first))
        nw = nw->left;
      else
        nw = nw->right;
    if (nw == nullptr)
      return end();
    else
      return iterator(nw, this);
  }
  const_iterator find(const Key &key) const {
    auto nw = root;
    while (nw &&
           (Compare{}(key, nw->data.first) || Compare{}(nw->data.first, key)))
      if (Compare{}(key, nw->data.first))
        nw = nw->left;
      else
        nw = nw->right;
    if (nw == nullptr)
      return cend();
    else
      return const_iterator(nw, this);
  }
};

} // namespace sjtu

#endif
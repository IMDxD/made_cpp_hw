#pragma once
#include <iterator>

namespace task {

template<class T>
struct ListNode {
  T* value;
  ListNode<T>* _node_prev;
  ListNode<T>* _node_next;
  void hook(ListNode<T>* position);
  void unhook();
};

template<class T, class Alloc = std::allocator<T>>
class list {

 private:

  using T_alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;
  using T_alloc_traits = std::allocator_traits<T_alloc_type>;
  using Node_alloc_type = typename T_alloc_traits::template rebind_alloc<ListNode<T>>;
  using Node_alloc_traits = std::allocator_traits<Node_alloc_type>;
  using Node = ListNode<T>;

 public:

  class iterator {
   public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator();
    explicit iterator(Node* node);
    iterator(const iterator&);
    iterator& operator=(const iterator&);

    iterator& operator++();
    iterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    iterator& operator--();
    iterator operator--(int);

    bool operator==(iterator other) const;
    bool operator!=(iterator other) const;

    Node* node_;
  };

  class const_iterator {
   public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator();
    explicit const_iterator(const Node* node);
    const_iterator(const iterator&);
    const_iterator& operator=(const const_iterator&);

    const_iterator& operator++();
    const_iterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    const_iterator& operator--();
    const_iterator operator--(int);

    bool operator==(const_iterator other) const;
    bool operator!=(const_iterator other) const;

    iterator get_iterator() const;

    const Node* node_;
  };

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;



 public:
  using value_type = T;
  using pointer = typename T_alloc_traits::pointer;
  using const_pointer = typename T_alloc_traits::const_pointer;
  using reference = typename T_alloc_traits::value_type&;
  using const_reference = const typename T_alloc_traits::value_type&;
  using size_type = typename T_alloc_traits::size_type;;
  using difference_type = typename T_alloc_traits::difference_type;
  using allocator_type = Alloc;

  list();
  explicit list(const Alloc& alloc);
  list(size_t count, const T& value, const Alloc& alloc = Alloc());
  explicit list(size_t count, const Alloc& alloc = Alloc());

  ~list();

  list(const list& other);
  list(list&& other);
  list& operator=(const list& other);
  list& operator=(list&& other);

  Alloc get_allocator() const;

  T& front();
  const T& front() const;

  T& back();
  const T& back() const;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  reverse_iterator rbegin();
  reverse_iterator rend();

  const_reverse_iterator crbegin() const;
  const_reverse_iterator crend() const;

  bool empty() const;
  size_t size() const;
  size_t max_size() const;
  void clear();

  iterator insert(const_iterator pos, const T& value);
  iterator insert(const_iterator pos, T&& value);
  iterator insert(const_iterator pos, size_t count, const T& value);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void push_back(const T& value);
  void push_back(T&& value);
  void pop_back();

  void push_front(const T& value);
  void push_front(T&& value);
  void pop_front();

  template<class... Args>
  iterator emplace(const_iterator pos, Args&& ... args);

  template<class... Args>
  void emplace_back(Args&& ... args);

  template<class... Args>
  void emplace_front(Args&& ... args);

  void resize(size_t count);
  void swap(list& other);

  void merge(list& other);
  void splice(const_iterator pos, list& other);
  void remove(const T& value);
  void reverse();
  void unique();
  void sort();

  void set_allocators(const Node_alloc_type&, const T_alloc_type&);

  // Your code goes here?..

 private:
  Node_alloc_type self_node_alloc;
  T_alloc_type self_value_alloc;
  Node main_node;
  size_type size_;

  void main_node_init();
  void set_size(size_type n);
  void increase_size(size_type n);
  void decrease_size(size_type n);
  pointer alloc_value();
  Node* alloc_node();
  void dealloc_value(pointer ptr);
  void dealloc_node(Node* ptr);
  const Node_alloc_type& get_node_allocator() const;
  template<class... Args>
  Node* create_node(Args&& ... args);
  void remove_node(Node* node);
  void move_nodes(list&& other);
  void clear_nodes();
  void fill_values_init(size_type n, const value_type& value);
  void default_values_init(size_type n);
  template<typename input_iterator>
  void iterator_init(input_iterator first, input_iterator last);
  template<class... Args>
  void insert_node(const_iterator pos, Args&& ... args);
  void erase(iterator pos);
};

template<class T>
void ListNode<T>::hook(ListNode<T>* position) {
  _node_prev = position->_node_prev;
  position->_node_prev->_node_next = this;
  _node_next = position;
  position->_node_prev = this;
}

template<class T>
void ListNode<T>::unhook() {
  _node_prev->_node_next = _node_next;
  _node_next->_node_prev = _node_prev;
}

template<class T, class Alloc>
list<T, Alloc>::iterator::iterator(): node_() {}

template<class T, class Alloc>
list<T, Alloc>::iterator::iterator(Node* node): node_(node) {}

template<class T, class Alloc>
list<T, Alloc>::iterator::iterator(const iterator& other): node_(other.node_) {}

template<class T, class Alloc>
typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator=(const iterator& other) {
  if (this == &other) {
    return *this;
  }
  this->node_ = other.node_;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator++() {
  node_ = node_->_node_next;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator++(int) {
  iterator tmp = *this;
  node_ = node_->_node_next;
  return tmp;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator::reference list<T, Alloc>::iterator::operator*() const {
  return *static_cast<Node*>(node_)->value;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator::pointer list<T, Alloc>::iterator::operator->() const {
  return static_cast<Node*>(node_)->value;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator--() {
  node_ = node_->_node_prev;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator--(int) {
  iterator tmp = *this;
  node_ = node_->_node_prev;
  return tmp;
}

template<class T, class Alloc>
bool list<T, Alloc>::iterator::operator==(const iterator other) const {
  return node_ == other.node_;
}

template<class T, class Alloc>
bool list<T, Alloc>::iterator::operator!=(const iterator other) const {
  return node_ != other.node_;
}

template<class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(): node_() {}

template<class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(const Node* node): node_(node) {}

template<class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(const iterator& other): node_(other.node_) {}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator=(const const_iterator& other) {
  if (this == &other) {
    return *this;
  }
  this->node_ = other.node_;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator++() {
  node_ = node_->_node_next;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator++(int) {
  iterator tmp = this->get_iterator();
  node_ = node_->_node_next;
  return tmp;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator::reference list<T, Alloc>::const_iterator::operator*() const {
  return *node_->value;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator::pointer list<T, Alloc>::const_iterator::operator->() const {
  return node_->value;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator--() {
  node_ = node_->_node_prev;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator--(int) {
  iterator tmp = *this;
  node_ = node_->_node_prev;
  return tmp;
}

template<class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator==(const const_iterator other) const {
  return node_ == other.node_;
}

template<class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator!=(const const_iterator other) const {
  return node_ != other.node_;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::const_iterator::get_iterator() const {
  return iterator(const_cast<Node*>(this->node_));
}

template<class T, class Alloc>
typename list<T, Alloc>::Node* list<T, Alloc>::alloc_node() {
  auto ptr = Node_alloc_traits::allocate(self_node_alloc, 1);
  return ptr;
}

template<class T, class Alloc>
typename list<T, Alloc>::pointer list<T, Alloc>::alloc_value() {
  auto ptr = T_alloc_traits::allocate(self_value_alloc, 1);
  return ptr;
}

template<class T, class Alloc>
void list<T, Alloc>::dealloc_value(pointer ptr) {
  T_alloc_traits::deallocate(self_value_alloc, ptr, 1);
}

template<class T, class Alloc>
void list<T, Alloc>::dealloc_node(Node* ptr) {
  Node_alloc_traits::deallocate(self_node_alloc, ptr, 1);
}

template<class T, class Alloc>
const typename list<T, Alloc>::Node_alloc_type& list<T, Alloc>::get_node_allocator() const { return self_node_alloc; }

template<class T, class Alloc>
void list<T, Alloc>::remove_node(list::Node* node) {
  pointer val = node->value;
  T_alloc_traits::destroy(self_value_alloc, val);
  dealloc_value(val);
  dealloc_node(node);
}

template<class T, class Alloc>
template<class... Args>
typename list<T, Alloc>::Node* list<T, Alloc>::create_node(Args&& ... args) {
  auto node_ptr = alloc_node();
  auto value_ptr = alloc_value();
  Node_alloc_traits::construct(self_node_alloc, value_ptr, std::forward<Args>(args)...);
  node_ptr->value = value_ptr;
  return node_ptr;
}

template<class T, class Alloc>
void list<T, Alloc>::move_nodes(list&& other) {
  Node* other_node = &other.main_node;
  if (other_node == other_node->_node_next) {
    main_node_init();
  } else {
    Node* this_node = &main_node;
    this_node->_node_next = other_node->_node_next;
    this_node->_node_prev = other_node->_node_prev;
    this_node->_node_next->_node_prev = this_node->_node_prev->_node_next = this_node;
    set_size(other.size());
    other.main_node_init();
  }
}

template<class T, class Alloc>
void list<T, Alloc>::clear_nodes() {
  Node* cur_node = main_node._node_next;
  Node* main_node_ptr = main_node._node_next->_node_prev;
  while (cur_node != main_node_ptr) {
    auto tmp = cur_node;
    cur_node = tmp->_node_next;
    remove_node(tmp);
  }
}
template<class T, class Alloc>
void list<T, Alloc>::set_size(size_type n) { size_ = n; }

template<class T, class Alloc>
void list<T, Alloc>::increase_size(list::size_type n) { size_ += n; }

template<class T, class Alloc>
void list<T, Alloc>::decrease_size(list::size_type n) { size_ -= n; }

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
  return iterator(&main_node);
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
  return iterator(main_node._node_next);
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
  return const_iterator(&main_node);
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
  return const_iterator(main_node._node_next);
}

template<class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
  return reverse_iterator(begin());
}

template<class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
  return reverse_iterator(end());
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const {
  return const_reverse_iterator(cbegin());
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crbegin() const {
  return const_reverse_iterator(cend());
}

template<class T, class Alloc>
void list<T, Alloc>::main_node_init() {
  main_node._node_prev = &main_node;
  main_node._node_next = &main_node;
  this->set_size(0);
}

template<class T, class Alloc>
template<class... Args>
void list<T, Alloc>::insert_node(const_iterator pos, Args&& ... args) {
  Node* tmp = create_node(std::forward<Args>(args)...);
  tmp->hook(const_cast<Node*>(pos.node_));
  increase_size(1);
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, const T& value) {
  insert_node(pos, value);
  iterator tmp = pos.get_iterator();
  return --tmp;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, T&& value) {
  insert_node(pos, std::move_if_noexcept(value));
  iterator tmp = pos.get_iterator();
  return --tmp;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, size_t count, const T& value) {
  for (size_type i(0); i < count; ++i) {
    insert_node(pos, value);
  }
  for (size_type i(0); i < count; ++i) {
    --pos;
  }
  return pos.get_iterator();
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(list::const_iterator pos) {
  iterator iter = iterator(pos.node_->_node_next);
  erase(pos.get_iterator());
  return iter;
}

template<class T, class Alloc>
void list<T, Alloc>::erase(list::iterator pos) {
  pos.node_->unhook();
  remove_node(pos.node_);
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(list::const_iterator first, list::const_iterator last) {
  while (first != last) {
    first = erase(first);
  }
  return last.get_iterator();
}

template<class T, class Alloc>
void list<T, Alloc>::push_back(const T& value) {
  insert_node(end(), value);
}

template<class T, class Alloc>
void list<T, Alloc>::push_back(T&& value) {
  insert_node(end(), std::move_if_noexcept(value));
}

template<class T, class Alloc>
void list<T, Alloc>::pop_back() {
  erase(iterator(main_node._node_prev));
}

template<class T, class Alloc>
void list<T, Alloc>::push_front(const T& value) {
  insert_node(begin(), value);
}

template<class T, class Alloc>
void list<T, Alloc>::push_front(T&& value) {
  insert_node(begin(), std::move_if_noexcept(value));
}

template<class T, class Alloc>
void list<T, Alloc>::pop_front() {
  erase(begin());
}

template<class T, class Alloc>
template<class... Args>
typename list<T, Alloc>::iterator list<T, Alloc>::emplace(list::const_iterator pos, Args&& ... args) {
  Node* tmp = create_node(std::forward<Args>(args)...);
  tmp->hook(const_cast<Node*>(pos.node_));
  increase_size(1);
  return iterator(tmp);
}

template<class T, class Alloc>
template<class... Args>
void list<T, Alloc>::emplace_back(Args&& ... args) {
  insert_node(end(), std::forward<Args>(args)...);
}

template<class T, class Alloc>
template<class... Args>
void list<T, Alloc>::emplace_front(Args&& ... args) {
  insert_node(begin(), std::forward<Args>(args)...);
}

template<class T, class Alloc>
void list<T, Alloc>::fill_values_init(list::size_type n, const value_type& value) {
  for (size_type i(0); i < n; ++i) {
    push_back(value);
  }
}

template<class T, class Alloc>
void list<T, Alloc>::default_values_init(list::size_type n) {
  for (size_type i(0); i < n; ++i) {
    emplace_back();
  }
}

template<class T, class Alloc>
template<typename input_iterator>
void list<T, Alloc>::iterator_init(input_iterator first, input_iterator last) {
  for (; first != last; ++first) {
    emplace_back(*first);
  }
}

template<class T, class Alloc>
list<T, Alloc>::list(): self_node_alloc(), self_value_alloc() { main_node_init(); }

template<class T, class Alloc>
list<T, Alloc>::list(const Alloc& alloc)
    : self_node_alloc(Node_alloc_type(alloc)), self_value_alloc(alloc) { main_node_init(); }

template<class T, class Alloc>
list<T, Alloc>::list(size_t count, const T& value, const Alloc& alloc):
    self_node_alloc(Node_alloc_type(alloc)),
    self_value_alloc(alloc) {
  main_node_init();
  fill_values_init(count, value);
}

template<class T, class Alloc>
list<T, Alloc>::list(size_t count, const Alloc& alloc):
    self_node_alloc(Node_alloc_type(alloc)),
    self_value_alloc(alloc) {
  main_node_init();
  default_values_init(count);
}

template<class T, class Alloc>
list<T, Alloc>::~list() {
  clear_nodes();
}
template<class T, class Alloc>
list<T, Alloc>::list(const list& other) :
    self_node_alloc(Node_alloc_traits::select_on_container_copy_construction(other.get_node_allocator())) {
  main_node_init();
  iterator_init(other.cbegin(), other.cend());
}
template<class T, class Alloc>
list<T, Alloc>::list(list&& other):
    self_node_alloc(std::move_if_noexcept(other.get_node_allocator())) {
  move_nodes(std::move(other));
}

template<class T, class Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(const list& other) {
  if (this == &other) {
    return *this;
  }
  auto other_alloc = other.get_allocator();
  clear();
  this->self_node_alloc = Node_alloc_traits::select_on_container_copy_construction(other_alloc);
  iterator_init(other.cbegin(), other.cend());
  return *this;
}

template<class T, class Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(list&& other) {
  move_nodes(std::move(other));
  return *this;
}

template<class T, class Alloc>
bool list<T, Alloc>::empty() const {
  return main_node._node_next == &main_node;
}

template<class T, class Alloc>
size_t list<T, Alloc>::size() const {
  return size_;
}

template<class T, class Alloc>
size_t list<T, Alloc>::max_size() const {
  return Node_alloc_traits::max_size(get_node_allocator());
}

template<class T, class Alloc>
void list<T, Alloc>::set_allocators(const Node_alloc_type& other_node_alloc, const T_alloc_type& other_value_alloc) {
  this->self_node_alloc = Node_alloc_traits::select_on_container_copy_construction(other_node_alloc);
  this->self_value_alloc = T_alloc_traits::select_on_container_copy_construction(other_value_alloc);
}

template<class T, class Alloc>
void list<T, Alloc>::clear() {
  clear_nodes();
  main_node_init();
}

template<class T, class Alloc>
void list<T, Alloc>::resize(size_t count) {
  const_iterator iter;
  const size_type length = size();
  if (count < length) {
    iter = cbegin();
    for (size_type i(0); i < count; ++i) {
      ++iter;
    }
  } else {
    iter = cend();
  }

  if (count > length)
    default_values_init(count - length);
  else
    erase(iter, end());
}

template<class T, class Alloc>
void list<T, Alloc>::swap(list& other) {
  std::swap(this->main_node, other.main_node);
  std::swap(other.self_node_alloc, this->self_node_alloc);
  std::swap(other.self_value_alloc, this->self_value_alloc);
}

template<class T, class Alloc>
void list<T, Alloc>::merge(list& other) {
  if (this != &other) {
    iterator this_first = begin();
    iterator this_last = end();
    iterator other_first = other.begin();
    iterator other_last = other.end();
    while (this_first != this_last && other_first != other_last) {
      if (*other_first < *this_first) {
        this->insert(this_first, other_first.operator*());
        ++other_first;
      } else
        ++this_first;
    }
    if (other_first != other_last) {
      while (other_first != other_last) {
        this->push_back(other_first.operator*());
        ++other_first;
      }
    }
    this->increase_size(other.size());
    other.set_size(0);
    other.main_node_init();
  }
}

template<class T, class Alloc>
void list<T, Alloc>::splice(list::const_iterator pos, list& other) {
  if (!other.empty()) {

    iterator other_first = other.begin();
    iterator other_last = other.end();
    while (other_first != other_last) {
      this->insert(pos, other_first.operator*());
    }
    this->increase_size(other.size());
    other.set_size(0);
    other.main_node_init();
  }
}

template<class T, class Alloc>
void list<T, Alloc>::remove(const T& value) {

  iterator first = begin();
  iterator last = end();
  iterator extra = last;
  while (first != last) {
    iterator next = first;
    ++next;
    if (*first == value) {
      if (&*first != &value)
        erase(first);
      else
        extra = first;
    }
    first = next;
  }
  if (extra != last)
    erase(extra);
}

template<class T, class Alloc>
void list<T, Alloc>::reverse() {
  Node* tmp = main_node._node_prev;
  main_node._node_prev = main_node._node_next;
  main_node._node_next = tmp;
}

template<class T, class Alloc>
void list<T, Alloc>::unique() {
  iterator first = begin();
  iterator last = end();
  if (first == last)
    return;
  iterator next = first;
  while (++next != last) {
    if (*first == *next) {
      erase(next);
    } else {
      first = next;
    }
    next = first;
  }
}

template<class T, class Alloc>
void list<T, Alloc>::sort() {

}
template<class T, class Alloc>
Alloc list<T, Alloc>::get_allocator() const {
  return self_value_alloc;
}

template<class T, class Alloc>
T& list<T, Alloc>::front() {
  return *begin();
}

template<class T, class Alloc>
const T& list<T, Alloc>::front() const {
  return *cbegin();
}

template<class T, class Alloc>
T& list<T, Alloc>::back() {
  return *(--end());
}

template<class T, class Alloc>
const T& list<T, Alloc>::back() const {
  return *(--cend());
}

}  // namespace task

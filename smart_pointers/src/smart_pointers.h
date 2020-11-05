#pragma once

namespace task {

template<class T>
class UniquePtr;
template<class T>
class SharedPtr;
template<class T>
class WeakPtr;

template<class T>
class UniquePtr {

 private:
  T* ptr;

 public:

  explicit UniquePtr(T* t);
  UniquePtr(UniquePtr<T>&& other) noexcept;
  ~UniquePtr();
  UniquePtr<T>& operator=(UniquePtr<T>&& other) noexcept;
  void swap(UniquePtr<T>& other) noexcept;
  T* get() const noexcept;
  T* release() noexcept;
  void reset(T* new_ptr = new T()) noexcept;
  T* operator->();
  typename std::add_lvalue_reference<T>::type operator*() const;

};

template<class T>
class SharedPtr {

 public:
  size_t* shared_counter;
  size_t* weak_counter;
  T* ptr;

  SharedPtr() noexcept;
  explicit SharedPtr(T* t);
  SharedPtr(const SharedPtr<T>& other) noexcept;
  SharedPtr(SharedPtr<T>&& other) noexcept;
  explicit SharedPtr(const WeakPtr<T>& other);
  SharedPtr<T>& operator=(const SharedPtr<T>& other) noexcept;
  SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept;
  ~SharedPtr();
  T* get() const noexcept;
  T* operator->();
  typename std::add_lvalue_reference<T>::type operator*() const;
  long use_count() const noexcept;
  void swap(SharedPtr<T>& other) noexcept;
  void reset() noexcept;
  void reset(T* new_ptr);

};

template<class T>
class WeakPtr {

 public:

  size_t* shared_counter;
  size_t* weak_counter;
  T* ptr;

  WeakPtr() noexcept;
  WeakPtr(const SharedPtr<T>& other) noexcept;
  WeakPtr(const WeakPtr<T>& other) noexcept;
  WeakPtr(WeakPtr<T>&& other) noexcept;
  WeakPtr<T>& operator=(const WeakPtr<T>& other) noexcept;
  WeakPtr<T>& operator=(const SharedPtr<T>& other) noexcept;
  WeakPtr<T>& operator=(WeakPtr<T>&& other) noexcept;
  ~WeakPtr();
  bool expired() const noexcept;
  long use_count() const noexcept;
  SharedPtr<T> lock() const noexcept;
  void reset() noexcept;
  void swap(WeakPtr<T>& other) noexcept;
};

template<class T>
UniquePtr<T>::UniquePtr(T* t): ptr(t) {}

template<class T>
UniquePtr<T>::UniquePtr(UniquePtr<T>&& other) noexcept:
    ptr(std::move_if_noexcept(other.ptr)) { other.ptr = nullptr; }

template<class T>
UniquePtr<T>::~UniquePtr() {
  delete this->ptr;
}

template<class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  this->ptr = std::move_if_noexcept(other.ptr);
  other.ptr = nullptr;
  return *this;
}

template<class T>
void UniquePtr<T>::swap(UniquePtr<T>& other) noexcept {
  std::swap(*this->ptr, *other.ptr);
}

template<class T>
T* UniquePtr<T>::get() const noexcept {
  return this->ptr;
}

template<class T>
T* UniquePtr<T>::release() noexcept {
  delete this->ptr;
  this->ptr = nullptr;
}

template<class T>
void UniquePtr<T>::reset(T* new_ptr) noexcept {
  delete this->ptr;
  this->ptr = new_ptr;
}

template<class T>
T* UniquePtr<T>::operator->() {
  return this->get();
}

template<class T>
typename std::add_lvalue_reference<T>::type UniquePtr<T>::operator*() const {
  return *this->get();
}

template<class T>
SharedPtr<T>::SharedPtr() noexcept: ptr(nullptr), shared_counter(new size_t(1)), weak_counter(new size_t(0)) {};

template<class T>
SharedPtr<T>::SharedPtr(T* t): ptr(t), shared_counter(new size_t(1)), weak_counter(new size_t(0)) {}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) noexcept:
    ptr(other.ptr),
    shared_counter(other.shared_counter),
    weak_counter(other.weak_counter) {
  *this->shared_counter += 1;
}

template<class T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept:
    ptr(std::move_if_noexcept(other.ptr)),
    shared_counter(std::move_if_noexcept(other.shared_counter)),
    weak_counter(std::move_if_noexcept(other.weak_counter)) {
  other.ptr = nullptr;
  other.shared_counter = new size_t (1);
  other.weak_counter = new size_t (0);
}

template<class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& other):
    ptr(other.ptr),
    shared_counter(other.shared_counter),
    weak_counter(other.weak_counter) {
  *this->shared_counter += 1;
}

template<class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) noexcept {
  if (this == &other) {
    return *this;
  }
  if (this->shared_counter > 0) {
    *this->shared_counter -= 1;
    if (this->shared_counter == 0){
      delete this->ptr;
      delete this->shared_counter;
      delete this->weak_counter;
    }
  }
  this->ptr = other.ptr;
  this->shared_counter = other.shared_counter;
  this->weak_counter = other.weak_counter;
  *this->shared_counter += 1;
  return *this;
}

template<class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  if (this->shared_counter > 0) {
    *this->shared_counter -= 1;
    if (this->shared_counter == 0){
      delete this->ptr;
      delete this->shared_counter;
      delete this->weak_counter;
    }
  }
  this->ptr = std::move_if_noexcept(other.ptr);
  this->shared_counter = std::move_if_noexcept(other.shared_counter);
  this->weak_counter = std::move_if_noexcept(other.weak_counter);
  other.ptr = nullptr;
  other.shared_counter = new size_t (1);
  other.weak_counter = new size_t (0);
  return *this;
}

template<class T>
SharedPtr<T>::~SharedPtr() {
  if (this->shared_counter > 0) {
    *this->shared_counter -= 1;
  }
  if (*this->shared_counter == 0) {
    delete this->ptr;
    if (*this->weak_counter == 0){
      delete this->shared_counter;
      delete this->weak_counter;
    }
  }
}

template<class T>
T* SharedPtr<T>::get() const noexcept {
  return this->ptr;
}

template<class T>
T* SharedPtr<T>::operator->() {
  return this->ptr;
}

template<class T>
typename std::add_lvalue_reference<T>::type SharedPtr<T>::operator*() const {
  return *this->ptr;
}

template<class T>
long SharedPtr<T>::use_count() const noexcept {
  return static_cast<long>(*this->shared_counter);
}

template<class T>
void SharedPtr<T>::swap(SharedPtr<T>& other) noexcept {
  std::swap(this->ptr, other.ptr);
  std::swap(this->shared_counter, other.shared_counter);
  std::swap(this->weak_counter, other.weak_counter);
}

template<class T>
void SharedPtr<T>::reset() noexcept {
  SharedPtr<T>().swap(*this);
}

template<class T>
void SharedPtr<T>::reset(T* new_ptr) {
  SharedPtr<T>(new_ptr).swap(*this);
}

template<class T>
WeakPtr<T>::WeakPtr() noexcept: ptr(nullptr), shared_counter(new size_t(0)), weak_counter(new size_t(1)) {};

template<class T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& other) noexcept:
    ptr(other.ptr),
    shared_counter(other.shared_counter),
    weak_counter(other.weak_counter) {
  *this->weak_counter += 1;
}

template<class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& other) noexcept:
    ptr(other.get()),
    shared_counter(other.shared_counter),
    weak_counter(other.weak_counter) {
  *this->weak_counter += 1;
}

template<class T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& other) noexcept:
    ptr(std::move_if_noexcept(other.ptr)),
    shared_counter(std::move_if_noexcept(other.shared_counter)),
    weak_counter(std::move_if_noexcept(other.weak_counter)) {
  other.ptr = nullptr;
  other.shared_counter = new size_t (0);
  other.weak_counter = new size_t (1);
}

template<class T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other) noexcept {
  if (this == other) {
    return *this;
  }
  if (this->shared_counter > 0) {
    *this->weak_counter -= 1;
  }
  this->ptr = other.ptr;
  this->shared_counter = other.shared_counter;
  this->weak_counter = other.weak_counter;
  *this->weak_counter += 1;
  return *this;
}

template<class T>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& other) noexcept {
  if (this->ptr == other.get()) {
    return *this;
  }
  if (*this->weak_counter > 0){
    *this->weak_counter -= 1;
  }
  this->ptr = other.get();
  this->shared_counter = other.shared_counter;
  this->weak_counter = other.weak_counter;
  *this->weak_counter += 1;
  return *this;
}

template<class T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other) noexcept {
  if (this == other) {
    return *this;
  }
  if (*this->weak_counter > 0){
    *this->weak_counter -= 1;
  }
  this->ptr = std::move_if_noexcept(other.ptr);
  this->shared_counter = std::move_if_noexcept(other.shared_counter);
  this->weak_counter = std::move_if_noexcept(other.weak_counter);
  other.ptr = nullptr;
  other.shared_counter = new size_t (0);
  other.weak_counter = new size_t (1);
  return *this;
}

template<class T>
WeakPtr<T>::~WeakPtr<T>() {
  if (*this->weak_counter > 0) {
    *this->weak_counter -= 1;
  }
  if (*this->weak_counter == 0 && *this->shared_counter == 0){
    delete this->weak_counter;
    delete this->shared_counter;
  }
}

template<class T>
bool WeakPtr<T>::expired() const noexcept {
  return *this->shared_counter == 0;
}

template<class T>
long WeakPtr<T>::use_count() const noexcept {
  return static_cast<long>(*this->shared_counter);
}

template<class T>
SharedPtr<T> WeakPtr<T>::lock() const noexcept {
  return SharedPtr<T>(*this);
}

template<class T>
void WeakPtr<T>::swap(WeakPtr<T>& other) noexcept {
  std::swap(this->ptr, other.ptr);
  std::swap(this->shared_counter, other.shared_counter);
  std::swap(this->weak_counter, other.weak_counter);
}

template<class T>
void WeakPtr<T>::reset() noexcept {
  WeakPtr<T>().swap(*this);
}

}

#include "smart_pointers.tpp"

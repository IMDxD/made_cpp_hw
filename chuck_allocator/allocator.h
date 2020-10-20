#pragma once

#include <cstddef>

struct Chunk {

  size_t offset;
  size_t total_size;
  char* buffer;
  Chunk* previous_node;

  explicit Chunk(size_t total_size) :
      offset(0),
      total_size(total_size),
      buffer(new char[total_size]),
      previous_node(nullptr) {}

};

template<class T>
class ChunkAllocator {
 public:

  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template<class U>
  struct rebind { typedef ChunkAllocator<U> other; };
  Chunk* cur_node;
  size_t chunk_size;
  size_t* copies_count;

  explicit ChunkAllocator(size_t new_chunk_size) {
    chunk_size = new_chunk_size;
    cur_node = nullptr;
    copies_count = new size_t;
    (*copies_count) = 1;
  };

  ChunkAllocator(const ChunkAllocator<T>& copy) {
    chunk_size = copy.chunk_size;
    cur_node = copy.cur_node;
    copies_count = copy.copies_count;
    (*copies_count) += 1;
  };

  ~ChunkAllocator() {
    if (*this->copies_count > 1) {
      (*this->copies_count) -= 1;
    } else {
      Chunk* prev_node;
      while (cur_node != nullptr) {
        prev_node = cur_node;
        cur_node = cur_node->previous_node;
        delete prev_node;
      }
      delete copies_count;
    }
  };

  ChunkAllocator& operator=(const ChunkAllocator& other) {
    if (this == &other) {
      return *this;
    }
    chunk_size = other.chunk_size;
    Chunk* prev_node;
    while (cur_node != nullptr) {
      prev_node = cur_node;
      cur_node = cur_node->previous_node;
      delete prev_node;
    }
    cur_node = other.cur_node;
    copies_count = other.copies_count;
    (*copies_count) += 1;
  };

  T* allocate(std::size_t n) {
    size_t allocate_memory = n * sizeof(T);
    Chunk* tmp_chunk = this->cur_node;
    while (tmp_chunk != nullptr) {
      if (tmp_chunk->offset + allocate_memory < tmp_chunk->total_size) {
        break;
      }
      tmp_chunk = tmp_chunk->previous_node;
    }
    if (tmp_chunk == nullptr) {
      tmp_chunk = new Chunk(this->chunk_size);
      tmp_chunk->previous_node = this->cur_node;
      this->cur_node = tmp_chunk;
    }
    auto return_pointer = tmp_chunk->buffer + tmp_chunk->offset;
    tmp_chunk->offset += allocate_memory;
    auto casted = reinterpret_cast<T*>(return_pointer);
    return casted;
  };

  void deallocate(T* p, std::size_t n) {};

  template<class... Args>
  void construct(T* p, Args&& ... args) {
    new(p) T(args...);
  };

  void destroy(T* p) {
    p->~T();
  };

};
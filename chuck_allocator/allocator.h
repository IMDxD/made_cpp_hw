#pragma once

#include <cstddef>

const size_t CHUNK_SIZE = 10 * 1024 * 1024;

struct Chunk {

  size_t offset;
  size_t total_size;
  char* buffer;
  Chunk* previous_node;
  size_t* copies_count;

  explicit Chunk() {
    offset = 0;
    total_size = CHUNK_SIZE;
    buffer = new char[total_size];
    previous_node = nullptr;
    copies_count = new size_t(1);
  }

  Chunk(const Chunk& copy) {
    this->offset = copy.offset;
    this->total_size = copy.total_size;
    this->previous_node = copy.previous_node;
    this->copies_count = copy.copies_count;
    this->buffer = copy.buffer;
    Chunk* cur_chunk = this;
    while (cur_chunk != nullptr) {
      (*cur_chunk->copies_count) += 1;
      cur_chunk = cur_chunk->previous_node;
    }
  }

  Chunk& operator=(const Chunk& other) {
    if (this == &other) {
      return *this;
    }
    remove_buffer();
    this->offset = other.offset;
    this->total_size = other.total_size;
    this->previous_node = other.previous_node;
    this->copies_count = other.copies_count;
    this->buffer = other.buffer;
    Chunk* cur_chunk = this;
    while (cur_chunk != nullptr) {
      (*cur_chunk->copies_count) += 1;
      cur_chunk = cur_chunk->previous_node;
    }
    return *this;
  }

  ~Chunk() {
    remove_buffer();
  }

 private:

  void remove_buffer() {
    Chunk* cur_chunk = this;
    while (cur_chunk != nullptr) {
      if (*cur_chunk->copies_count == 1) {
        delete[] cur_chunk->buffer;
        delete cur_chunk->copies_count;
      } else {
        (*cur_chunk->copies_count) -= 1;
      }
      cur_chunk = cur_chunk->previous_node;
    }
  }

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

  explicit ChunkAllocator() {
    chunk_size = CHUNK_SIZE;
    last_chunk = nullptr;
  };

  ChunkAllocator(const ChunkAllocator<value_type>& copy) {
    chunk_size = copy.chunk_size;
    last_chunk = new Chunk(*copy.last_chunk);
  };

  ~ChunkAllocator() {
    delete last_chunk;
  };

  ChunkAllocator& operator=(const ChunkAllocator& other) {
    if (this == &other) {
      return *this;
    }
    last_chunk = *(*last_chunk = *other.last_chunk);
  };

  pointer allocate(std::size_t n) {
    size_t allocate_memory = n * sizeof(value_type);
    Chunk* tmp_chunk = this->last_chunk;
    while (tmp_chunk != nullptr) {
      if (tmp_chunk->offset + allocate_memory < tmp_chunk->total_size) {
        break;
      }
      tmp_chunk = tmp_chunk->previous_node;
    }
    if (tmp_chunk == nullptr) {
      tmp_chunk = new Chunk();
      tmp_chunk->previous_node = this->last_chunk;
      this->last_chunk = tmp_chunk;
    }
    auto return_pointer = tmp_chunk->buffer + tmp_chunk->offset;
    tmp_chunk->offset += allocate_memory;
    auto casted = reinterpret_cast<pointer>(return_pointer);
    return casted;
  };

  void deallocate(pointer p, std::size_t n) {};

  template<class... Args>
  void construct(pointer p, Args&& ... args) {
    new(p) value_type(args...);
  };

  void destroy(pointer p) {
    p->~value_type();
  };

 private:

  Chunk* last_chunk;
  size_t chunk_size;
};
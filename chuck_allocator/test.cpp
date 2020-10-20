#include "allocator.h"
#include "iostream"
#include "vector"

struct Test {
  int a = 10;
  double b = 3;

  Test(int new_a, double new_b) : a(new_a), b(new_b) {};
  ~Test() {
    a = 0;
    b = 0.0;
  }
};

void FailWithMsg(const std::string& msg, int line) {
  std::cerr << "Test failed!\n";
  std::cerr << "[Line " << line << "] " << msg << std::endl;
  std::exit(EXIT_FAILURE);
}

#define ASSERT_TRUE_MSG(cond, msg) \
    if (!(cond)) {FailWithMsg(msg, __LINE__);};

int main() {
  ChunkAllocator<Test>::value_type a(1, 2);
  ASSERT_TRUE_MSG(a.a == 1 && a.b == 2.0, "Wrong value_type");

  ChunkAllocator<Test>::pointer b = new Test(3, 4);
  new(b) Test(1, 2);
  ASSERT_TRUE_MSG(b->a == 1 && b->b == 2.0, "Wrong pointer type");

  ChunkAllocator<Test>::reference c = a;
  c.a = 10;
  ASSERT_TRUE_MSG(a.a == 10, "Wrong reference type");

  ChunkAllocator<Test>::const_pointer d = new Test(3, 4);
  ChunkAllocator<Test>::const_reference e = a;

  Test& f = const_cast<Test&>(e);
  f.b = 100;
  ASSERT_TRUE_MSG(f.b == 100, "Wrong const reference type");

  Test* g = const_cast<Test*>(d);
  g->b = 100;
  ASSERT_TRUE_MSG(g->b == 100, "Wrong const pointer type");

  auto alloc1 = new ChunkAllocator<Test>(128);
  ASSERT_TRUE_MSG((*alloc1->copies_count) == 1, "Wrong initial count");

  auto alloc2 = new ChunkAllocator<Test>(*alloc1);
  ASSERT_TRUE_MSG((*alloc1->copies_count) == 2, "Wrong alloc 1 copies count");
  ASSERT_TRUE_MSG((*alloc2->copies_count) == 2, "Wrong alloc 2 copies count");

  delete alloc1;
  ASSERT_TRUE_MSG((*alloc2->copies_count) == 1, "Wrong alloc 2 copies count");

  auto alloc3 = new ChunkAllocator<Test>(*alloc2);
  ASSERT_TRUE_MSG((*alloc2->copies_count) == 2, "Wrong alloc 2 copies count");
  ASSERT_TRUE_MSG((*alloc3->copies_count) == 2, "Wrong alloc 2 copies count");

  auto* alloc4 = new ChunkAllocator<Test>(128);

  *alloc4 = *alloc2;
  ASSERT_TRUE_MSG((*alloc2->copies_count) == 3, "Wrong alloc 2 copies count");
  ASSERT_TRUE_MSG((*alloc3->copies_count) == 3, "Wrong alloc 2 copies count");
  ASSERT_TRUE_MSG((*alloc4->copies_count) == 3, "Wrong alloc 2 copies count");

  delete alloc2;
  delete alloc3;
  alloc4->cur_node = new Chunk(128);
  delete alloc4;
  ASSERT_TRUE_MSG(alloc4->cur_node->total_size == 0, "Wrong full destruction");

  auto alloc = new ChunkAllocator<Test>(1024);
  Test* test_array1 = alloc->allocate(4);
  Test* test_array2 = alloc->allocate(4);
  size_t t = sizeof(Test);
  ASSERT_TRUE_MSG(test_array2 == test_array1 + 4, "Wrong allocation");
  alloc->construct(test_array1, 1, 2.0);
  ASSERT_TRUE_MSG(test_array1->a == 1 && test_array1->b == 2.0, "Wrong construction");
  alloc->destroy(test_array1);
  ASSERT_TRUE_MSG(test_array1->a == 0 && test_array1->b == 0.0, "Wrong construction");
  Test* test_array3 = alloc->allocate(63);
  ASSERT_TRUE_MSG(test_array3 != test_array1 + 64, "Wrong allocation");
  Test* test_array4 = alloc->allocate(40);
  ASSERT_TRUE_MSG(test_array4 == test_array1 + 8, "Wrong allocation");
  std::vector<Test*> tmp_v(10);
  for (auto& p: tmp_v) {
    p = alloc->allocate(63);
  }
  Test* test_array5 = alloc->allocate(2);
  ASSERT_TRUE_MSG(test_array5 == test_array1 + 48, "Wrong allocation");
  auto alloc_destruct2 = alloc;
  delete alloc_destruct2;
  alloc->construct(test_array1, 1, 2.0);
  ASSERT_TRUE_MSG(test_array1->a == 1 && test_array1->b == 2.0, "Wrong copy destruction");
  delete alloc;
}
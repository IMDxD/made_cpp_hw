#ifndef MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H
#define MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H

#include "type_list.h"
#include "value_list.h"
#include "graph.h"

template<size_t, typename...>
struct Used {};

template<size_t i>
struct Used<i> {

 public:
  using type = typename Used<i - 1, ValueList<false>>::type;
};

template<size_t i, Type Head, Type... Tail>
struct Used<i, ValueList<Head, Tail...>> {

 private:
  using List = ValueList<Head, Tail...>;

 public:
  using type = typename Used<i - 1, typename PushBack<false, List>::type>::type;
};

template<Type Head, Type... Tail>
struct Used<0, ValueList<Head, Tail...>> {

 public:
  using type = ValueList<Head, Tail...>;
};

template<typename ...>
struct Merge {};

template<typename Right>
struct Merge<NullType, Right> {
  using type = Right;
};

template<typename Left>
struct Merge<Left, NullType> {
  using type = Left;
};

template<typename Left, typename Right>
struct Merge<Left, Right> {
  using type = typename Concat<typename Left::Head, typename Merge<typename Left::Tail, Right>::type>::type;
};

template<typename Vertices, typename VertUsed, size_t end, bool is_used, bool is_same, bool computed, typename Edges>
struct DFS {};

template<typename Vertices, typename VertUsed, size_t end>
struct DFS<Vertices, VertUsed, end, true, false, true, NullType> {

  static constexpr bool value = false;
};

template<typename Vertices, typename VertUsed, size_t end>
struct DFS<Vertices, VertUsed, end, false, false, true, NullType> {

  static constexpr bool value = false;
};

template<typename Vertices, typename VertUsed, size_t end, bool computed, typename Edges>
struct DFS<Vertices, VertUsed, end, false, true, computed, Edges> {

  static constexpr bool value = true;
};

template<typename Vertices, typename VertUsed, size_t end, typename Edges>
struct DFS<Vertices, VertUsed, end, false, false, true, Edges> {

 private:

  using new_vertex = typename TypeAt<Edges::Head::to, Vertices>::type;
  using new_edges = typename Merge<typename new_vertex::Edges, Edges>::type;
  using updated_used = typename Set<new_vertex::index, true, VertUsed>::type;
  static constexpr bool is_used = Get<new_vertex::index, VertUsed>::value;
  static constexpr bool is_same = new_vertex::index == end;

 public:

  static constexpr bool value = DFS<Vertices, updated_used, end, is_used, is_same, false, new_edges>::value;
};

template<typename Vertices, typename VertUsed, size_t end, typename Edges>
struct DFS<Vertices, VertUsed, end, false, false, false, Edges> {

 public:

  static constexpr bool value = DFS<Vertices, VertUsed, end, false, false, true, Edges>::value;

};

template<typename Vertices, typename VertUsed, size_t end, typename Edges>
struct DFS<Vertices, VertUsed, end, true, false, true, Edges> {

 private:

  static constexpr bool is_used = Get<Edges::Head::to, VertUsed>::value;
  static constexpr bool is_same = Edges::Head::to == end;

 public:

  static constexpr bool value = DFS<Vertices, VertUsed, end, is_used, is_same, false, Edges>::value;
};

template<typename Vertices, typename VertUsed, size_t end, typename Edges>
struct DFS<Vertices, VertUsed, end, true, false, false, Edges> {

 private:

  using new_edges = typename Edges::Tail;

 public:

  static constexpr bool value = DFS<Vertices, VertUsed, end, true, false, true, new_edges>::value;
};

template<typename Graph, size_t start, size_t end>
struct PathExists {

 private:
  using vert_list = typename Graph::Vertices;
  using vert_used = typename Set<start, true, typename Used<Length<vert_list>::value>::type>::type;
  using vert = typename TypeAt<start, vert_list>::type;
  using vert_edges = typename vert::Edges;

 public:
  static constexpr bool value = DFS<vert_list, vert_used, end, false, start == end, true, vert_edges>::value;
};

#endif /// MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H.

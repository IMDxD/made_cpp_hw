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

template<size_t, bool, typename, typename>
struct GetVertex {};

template<size_t Value, typename Head, typename Vertices>
struct GetVertex<Value, false, Head, Vertices> {

 public:
  using type = typename GetVertex<Value, Vertices::Head::index == Value, typename Vertices::Head,
                                  typename Vertices::Tail>::type;
};

template<size_t Value, typename Head, typename Vertices>
struct GetVertex<Value, true, Head, Vertices> {

 public:
  using type = Head;
};

template<typename Vertices, typename VertUsed, typename EdgesHead, typename EdgesTail,
    size_t end, bool is_used, bool is_same, typename Head,
    typename Tail>
struct DFS {};

template<typename Vertices, typename VertUsed, typename EdgesHead, typename EdgesTail,
    size_t end, typename Head, typename Tail>
struct DFS<Vertices, VertUsed, EdgesHead, EdgesTail, end, false, false, Head, Tail> {

 private:
  using vertex_to = typename GetVertex<EdgesHead::to, false, typename Vertices::Head, Vertices>::type;
  using vertex_to_head_edge = typename vertex_to::Edges::Head;
  using vertex_to_tail_edges = typename vertex_to::Edges::Tail;
  static constexpr bool edge_to_is_used = Get<vertex_to_head_edge::to, VertUsed>::value;
  using updated_used = typename Set<EdgesHead::from, true, VertUsed>::type;

 public:
  static constexpr bool value = DFS<Vertices, updated_used, vertex_to_head_edge,
                                    vertex_to_tail_edges, end, edge_to_is_used,
                                    EdgesHead::to == end, vertex_to, TypeList<Head, Tail>>::value;
};

template<typename Vertices, typename VertUsed, typename EdgesHead, typename EdgesTail,
    size_t end, typename Head, typename Tail>
struct DFS<Vertices, VertUsed, EdgesHead, EdgesTail, end, true, false, Head, Tail> {

 private:
  using edge = typename EdgesTail::Head;
  using edge_tail = typename EdgesTail::Tail;
  static constexpr bool edge_to_is_used = Get<edge::to, VertUsed>::value;

 public:
  static constexpr bool value = DFS<Vertices, VertUsed, edge, edge_tail, end, edge_to_is_used,
                                    edge::to == end, Head, Tail>::value;
};

template<typename Vertices, typename VertUsed, typename EdgesHead, size_t end, typename Head, typename Tail>
struct DFS<Vertices, VertUsed, EdgesHead, NullType, end, true, false, Head, Tail> {

 private:
  using new_head = typename Tail::Head;
  using new_tail = typename Tail::Tail;
  using head_edges = typename Head::Edges;
  using edge = typename head_edges::Head;
  using tail_edges = typename head_edges::Tail;
  static constexpr bool edge_to_is_used = Get<edge::to, VertUsed>::value;
  using updated_used = typename Set<EdgesHead::from, true, VertUsed>::type;

 public:
  static constexpr bool value = DFS<Vertices, updated_used, edge, tail_edges,
                                    end, edge_to_is_used, edge::to == end,
                                    new_head, new_tail>::value;
};

template<typename Vertices, typename VertUsed, typename EdgesHead, typename EdgesTail,
    size_t end, typename Head, typename Tail>
struct DFS<Vertices, VertUsed, EdgesHead, EdgesTail, end, false, true, Head, Tail> {

 public:
  static constexpr bool value = true;
};

template<typename Vertices, typename VertUsed, typename EdgesHead, typename EdgesTail, size_t end, typename Head>
struct DFS<Vertices, VertUsed, EdgesHead, EdgesTail, end, true, false, Head, NullType> {

 public:
  static constexpr bool value = false;

};

template<typename Graph, size_t start, size_t end>
struct PathExists {

 private:
  using vert_list = typename Graph::Vertices;
  using vert_used = typename Used<Length<vert_list>::value>::type;
  using vert = typename GetVertex<start, false, typename vert_list::Head, vert_list>::type;
  using vert_edges = typename vert::Edges;
  static constexpr bool is_end = vert_edges::Head::to == end;

  public:
  static constexpr bool value = DFS<vert_list, vert_used, typename vert_edges::Head, typename vert_edges::Tail,
                                    end, false, is_end, vert, NullType>::value;
};

#endif /// MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H.

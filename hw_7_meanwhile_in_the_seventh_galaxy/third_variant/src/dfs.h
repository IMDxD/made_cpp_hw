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


template<typename Vertices, typename VertUsed,
    size_t end, bool is_used, bool is_same, typename VertexWayHead, typename VertexWayTail, typename Edges>
struct DFS {};

template<typename Vertices, typename VertUsed, size_t end, typename VertexWayHead, typename VertexWayTail,
    typename Edges>
struct DFS<Vertices, VertUsed, end, false, false, VertexWayHead, VertexWayTail, Edges> {

 private:
  using first_edge = typename Edges::Head;
  using vertex_to = typename TypeAt<first_edge::to, Vertices>::type;
  using vertex_edges = typename vertex_to::Edges;
  static constexpr bool edge_to_is_used = Get<vertex_to::index, VertUsed>::value;
  using updated_used = typename Set<VertexWayHead::index, true, VertUsed>::type;
  using updated_way = typename Concat<VertexWayHead, VertexWayTail>::type;

 public:
  static constexpr bool value = DFS<Vertices, updated_used, end, edge_to_is_used, vertex_to::index == end,
                                    vertex_to, updated_way, vertex_edges >::value;
};

template<typename Vertices, typename VertUsed, size_t end, typename VertexWayHead, typename VertexWayTail>
struct DFS<Vertices, VertUsed, end, false, false, VertexWayHead, VertexWayTail, NullType> {

 private:
  using updated_way_head = typename VertexWayTail::Head;
  using updated_way_tail = typename VertexWayTail::Tail;
  using edges = typename updated_way_head::Edges;
  using updated_used = typename Set<VertexWayHead::index, true, VertUsed>::type;

 public:
  static constexpr bool value = DFS<Vertices, updated_used, end, true, false,
                                    updated_way_head, updated_way_tail, edges>::value;
};


template<typename Vertices, typename VertUsed, size_t end, typename VertexWayHead, typename VertexWayTail>
struct DFS<Vertices, VertUsed, end, true, false, VertexWayHead, VertexWayTail, NullType> {

 private:
  using updated_way_head = typename VertexWayTail::Head;
  using updated_way_tail = typename VertexWayTail::Tail;
  using edges = typename updated_way_head::Edges;
  using updated_used = typename Set<VertexWayHead::index, true, VertUsed>::type;

 public:
  static constexpr bool value = DFS<Vertices, updated_used, end, true, false,
                                    updated_way_head, updated_way_tail, edges>::value;
};


template<typename Vertices, typename VertUsed, size_t end, typename VertexWayHead, typename VertexWayTail,
    typename Edges>
struct DFS<Vertices, VertUsed, end, true, false, VertexWayHead, VertexWayTail, Edges> {

 private:
  using edge = typename Edges::Head;
  using edge_tail = typename Edges::Tail;
  static constexpr bool edge_to_is_used = Get<edge::to, VertUsed>::value;

 public:
  static constexpr bool value = DFS<Vertices, VertUsed, end, edge_to_is_used,
                                    edge::to == end, VertexWayHead, VertexWayTail, edge_tail>::value;
};


template<typename Vertices, typename VertUsed, size_t end, typename VertexWayHead, typename VertexWayTail,
     typename Edges>
struct DFS<Vertices, VertUsed, end, false, true, VertexWayHead, VertexWayTail, Edges> {

 public:
  static constexpr bool value = true;
};

template<typename Vertices, typename VertUsed, size_t end, typename VertexWayHead>
struct DFS<Vertices, VertUsed, end, true, false, VertexWayHead, NullType, NullType> {

 public:
  static constexpr bool value = false;

};


template<typename Graph, size_t start, size_t end>
struct PathExists {

 private:
  using vert_list = typename Graph::Vertices;
  using vert_used = typename Used<Length<vert_list>::value>::type;
  using vert = typename TypeAt<start, vert_list>::type;
  using vert_edges = typename vert::Edges;

  public:
  static constexpr bool value = DFS<vert_list, vert_used, end, false, start == end,
                                    vert, NullType, vert_edges>::value;
};

#endif /// MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H.

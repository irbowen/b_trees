#ifndef INNER_NODE_H
#define INNER_NODE_H

#include <cassert>
#include <mutex>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <typeinfo>

#include "node.h"

/* Class for the inner nodes in the tree */
class Inner_Node : public Node {
  /* */
  std::shared_timed_mutex node_mutex;
  std::mutex m;
public:
  Inner_Node();
  std::vector<int> keys;
  std::vector<Node*> values;
  int get_value(int);
  int unique_id;
  void create_first_node(int, int);
  bool add_key_value_pair(int, int, Node_key&);
  void add_to_child(std::vector<Node*>::iterator, int, int);
  void add_vector_keys(std::vector<int>);
  void add_vector_nodes(std::vector<Node*>);
  void add_key(int);
  void print_keys();
  std::string print_r(int);
  bool can_split();
  bool is_inner();
};

#endif

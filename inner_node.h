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
public:
  Inner_Node();
  std::list<int> keys;
  std::list<Node*> values;
  int get_value(int);
  int unique_id;
  void create_first_node(int, int);
  bool add_key_value_pair(int, int, Node_key&);
  void add_to_child(std::list<Node*>::iterator, int, int);
  void add_vector_keys(std::list<int>);
  void add_vector_nodes(std::list<Node*>);
  void add_key(int);
  void print_keys();
  std::string print_r(int);
};

#endif

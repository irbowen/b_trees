#ifndef INNER_NODE_H
#define INNER_NODE_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "node.h"

/* Class for the inner nodes in the tree */
class Inner_Node : public Node {
  /* */
  void add_to_child(int, int, int);
  void create_first_node(int, int);
public:
  Inner_Node() {
    keys.reserve(FAN_OUT);
    values.reserve(FAN_OUT + 1);
  }
  std::vector<int> keys;
  std::vector<Node*> values;
  void print_keys();
  bool add_key_value_pair(int, int, Node_key&);
  void add_vector_keys(std::vector<int>);
  void add_vector_nodes(std::vector<Node*>);
  void add_key(int);
  void print_r();
};

#endif

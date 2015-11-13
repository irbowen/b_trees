#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

#include "node.h"

/* Class for the leaf nodes of the tree */
class Leaf_Node : public Node {
  std::vector<std::tuple<int, int>> elements;
public:
  Leaf_Node() {
    elements.reserve(DATA_SLOTS);
  }
  Leaf_Node(std::vector<std::tuple<int, int>> v) : elements(v) {}
  void add_vector(std::vector<std::tuple<int, int>>);
  bool add_key_value_pair(int, int, Node_key&);
  void print_r();
};

#endif
